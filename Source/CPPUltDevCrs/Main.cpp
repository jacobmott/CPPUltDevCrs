// Fill out your copyright notice in the Description page of Project Settings.


#include "Main.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "Weapon.h"
#include "Sound/SoundCue.h"

#include "Enemy.h"

#include "MainPlayerController.h"

#include "Math/NumericLimits.h"

#include "FirstSaveGame.h"

#include "ItemStorage.h"

// Sets default values
AMain::AMain()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;

  SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
  SpringArmComponent->SetupAttachment(GetRootComponent());
  SpringArmComponent->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
  SpringArmComponent->TargetArmLength = 600.0f;
  SpringArmComponent->bUsePawnControlRotation = true; // Rotate arm based on controller
  SpringArmComponent->bEnableCameraLag = true;
  SpringArmComponent->CameraLagSpeed = 3.0f;

  // Pulls toward player if theres a collision
  CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
  CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
  // Attach the camera to end of spring arm and let the spring arm adjust to match the controller oreientation
  CameraComponent->bUsePawnControlRotation = false; 

  GetCapsuleComponent()->SetCapsuleSize(50.0f, 105.0f);

  // Set our turn rates for input
  BaseTurnRate = 65.0f;
  BaseLookUpAtRate = 65.0f;

  //We modify the controller with the camera when we turn our mouse, but we dont want the
  //characters movement/rotation to also rotate when we rotate the camera
  //So, Don't rotate when the controller rotates
  //Let that just affect the camera
  bUseControllerRotationYaw = false;
  bUseControllerRotationPitch = false;
  bUseControllerRotationRoll = false;


  //Configure character movement
  //Character moves in the direction of input(left or right move)
  GetCharacterMovement()->bOrientRotationToMovement = true;
  GetCharacterMovement()->RotationRate = FRotator(0.0f, 640.0f, 0.0f); //At this rotation rate
  GetCharacterMovement()->JumpZVelocity = 650.0f;
  GetCharacterMovement()->AirControl = 0.4f;


  MaxHealth = 100.0f;
  Health = 65.0f;
  MaxStamina = 150.0f;
  Stamina = 120.0f;
  Coins = 0.0f;


  RunningSpeed = 650.0f;
  SprintingSpeed = 950.0f;
  bShiftKeyDown = false;
  bLMBDown = false;

  MovementStatus = EMovementStatus::EMS_Normal;
  StaminaStatus = EStaminaStatus::ESS_Normal;

  StaminaDrainRate = 25.0f;
  MinSprintStamina = 50.0f;

  bHasCombatTarget = false;

  bMovingForward = false;
  bMovingRight = false;

  bESCDown = false;
  

}

void AMain::ShowPickupLocations()
{
  //for (int32 i = 0; i < PickupLocations.Num(); i++) {
  //  UKismetSystemLibrary::DrawDebugSphere(this, PickupLocations[i], 25.0f, 8, FLinearColor::Green, 10.0f, 0.5f);
  //}

  for (FVector Location : PickupLocations) {
    UKismetSystemLibrary::DrawDebugSphere(this, Location, 25.0f, 8, FLinearColor::Green, 10.0f, 0.5f);
  }

}

void AMain::SetInterpToEnemy(bool Interp)
{
  bInterpToEnemy = Interp;
}

void AMain::UpdateCombatTarget()
{

  TArray<AActor*> OverlappingActors;
  GetOverlappingActors(OverlappingActors, EnemyFilter);
  if (OverlappingActors.Num() == 0) { 
    if(!MainPlayerController){return;}
    MainPlayerController->RemoveEnemyHealthBar();
    return;
  }
  
  AEnemy* ClosestEnemy = Cast<AEnemy>(OverlappingActors[0]);
  if (!ClosestEnemy){ return; }
  //Get dot product or magnitude (distance) 
  FVector MyLocation = GetActorLocation();
  float MinDistance = (ClosestEnemy->GetActorLocation() - MyLocation).Size();
  //float MinDistance =  TNumericLimits<float>::Max();

  for (auto Actor : OverlappingActors) {
    AEnemy* Enemy = Cast<AEnemy>(Actor);
    if (!Enemy){ continue; }
    float DistanceToActor = (Enemy->GetActorLocation() - MyLocation).Size();
    if (DistanceToActor < MinDistance) {
      MinDistance = DistanceToActor;
      ClosestEnemy = Enemy;
    }
  }

  if (MainPlayerController){ 
    MainPlayerController->DisplayEnemyHealthBar();
  }
  SetCombatTarget(ClosestEnemy);
  bHasCombatTarget = true;
  
}

FRotator AMain::GetLookAtRotationYaw(FVector Target)
{

  FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target);
  FRotator LookAtRotationYaw(0.0f, LookAtRotation.Yaw, 0.0f);
  return LookAtRotationYaw;
}

void AMain::SetMovementStatus(EMovementStatus Status)
{

  MovementStatus = Status;
  if (MovementStatus == EMovementStatus::EMS_Sprinting) {
    GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;
  }
  else {
    GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
  }
  
}

void AMain::ShiftKeyDown()
{
  bShiftKeyDown = true;
}

void AMain::ShiftKeyUp()
{
  bShiftKeyDown = false;
}

void AMain::Attack()
{

  if (MovementStatus == EMovementStatus::EMS_Dead) { return; }
  if (bAttacking){ return; }
  bAttacking = true;
  SetInterpToEnemy(true);

  UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
  if (AnimInstance && CombatMontage) {
    int32 Section = FMath::RandRange(0,1);
    switch (Section)
    {
    case 0:
      AnimInstance->Montage_Play(CombatMontage, 2.2f);
      AnimInstance->Montage_JumpToSection(FName("Attack_1"), CombatMontage);
      break;
    case 1:
      AnimInstance->Montage_Play(CombatMontage, 1.8f);
      AnimInstance->Montage_JumpToSection(FName("Attack_2"), CombatMontage);
      break;
    default:
      break;
    }

    //if (!EquippedWeapon) { return; }
    //if (!EquippedWeapon->SwingSound) { return; }
    //UGameplayStatics::PlaySound2D(this, EquippedWeapon->SwingSound);

  }

}

void AMain::AttackEnd()
{
  bAttacking = false;
  SetInterpToEnemy(false);
  if (bLMBDown) {
    Attack();
  }
}

void AMain::PlaySwingSound()
{
  if (!EquippedWeapon) { return; }
  if (!EquippedWeapon->SwingSound) { return; }
  UGameplayStatics::PlaySound2D(this, EquippedWeapon->SwingSound);
}

void AMain::SetEquippedWeapon(AWeapon* Weapon)
{

  if (EquippedWeapon) {
    EquippedWeapon->Destroy();
  }

  EquippedWeapon = Weapon;
}

void AMain::DecrementHealth(float Amount)
{ 

  Health -= Amount;
  if (Health <= 0.0f) {
    Die();
  }

}

void AMain::Die()
{
  //UE_LOG(LogTemp, Warning, TEXT("AMain: Die"));
  if (MovementStatus == EMovementStatus::EMS_Dead) { return; }
  UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
  if (AnimInstance && CombatMontage) {
    AnimInstance->Montage_Play(CombatMontage, 1.0f);
    AnimInstance->Montage_JumpToSection(FName("Death"), CombatMontage);
  }
  //UE_LOG(LogTemp, Warning, TEXT("AMain: Die SetMovementStatus(EMovementStatus::EMS_Dead)"));
  SetMovementStatus(EMovementStatus::EMS_Dead);
  
  //DeathEnd();
}

void AMain::DeathEnd()
{

  //UE_LOG(LogTemp, Warning, TEXT("AMain: DeathEnd"));
  GetMesh()->bPauseAnims = true;
  GetMesh()->bNoSkeletonUpdate = true;
}


void AMain::Jump()
{
  //UE_LOG(LogTemp, Warning, TEXT("AMain: Jump"));
  if (MainPlayerController) {
    if (MainPlayerController->bPauseMenuVisible) { return; }
  }
  if (MovementStatus != EMovementStatus::EMS_Dead) {
    //UE_LOG(LogTemp, Warning, TEXT("AMain: Jump MovementStatus != EMovementStatus::EMS_Dead"));
    Super::Jump();
  }
}

void AMain::ESCDown()
{
  bESCDown = true;
  if (!MainPlayerController){return;}
  MainPlayerController->TogglePauseMenu();
}

void AMain::ESCUp()
{
  bESCDown = false;
}

void AMain::IncrementCoins(int32 Amount)
{
  Coins += Amount;
}

void AMain::IncrementHealth(float Amount)
{
  Health += Amount;
  if (Health >= MaxHealth) {
    Health = MaxHealth;
  }
}

float AMain::TakeDamage(float DamageAmout, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{

  Health -= DamageAmout;
  if (Health <= 0.0f) {
    Die();
    AEnemy* Enemy = Cast<AEnemy>(DamageCauser);
    if (Enemy) {
      Enemy->bHasValidTarget = false;
    }
  }

  //DecrementHealth(DamageAmout);
  return DamageAmout;
}

void AMain::SwitchLevel(FName LevelName, bool AutoSave, bool bSavePosition)
{
  UE_LOG(LogTemp, Warning, TEXT("AMain: SwitchLevel1:"));
  UWorld* World = GetWorld();
  if (!World) { return; }

  FString CurrentLevel = World->GetMapName();
  CurrentLevel.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
  FString CurrLevelName = LevelName.ToString();

  FName CurrentLevelName(*CurrentLevel);
  FString MapName = LevelName.ToString();

  if (CurrentLevel.Equals(CurrLevelName)) { return; }

  if (AutoSave) {
    SaveGame(LevelName, bSavePosition);
    //AutoSaveDelegate.BindUFunction(this, FName("SaveGame"), LevelName, true);
    //GetWorldTimerManager().SetTimer(AutoSaveHandle, AutoSaveDelegate, 10.0f,false, 5.0f);
  }
  //if ( CurrentLevel.Equals(LevelName.ToString()) ){ return; }
  
  UGameplayStatics::OpenLevel(World, LevelName);

}

void AMain::SaveGame(FName LevelName, bool bSavePosition)
{
  UFirstSaveGame* SaveGameInstance = Cast<UFirstSaveGame>(UGameplayStatics::CreateSaveGameObject(UFirstSaveGame::StaticClass()));
  FDebug::DumpStackTraceToLog(ELogVerbosity::All);
  SaveGameInstance->CharacterStats.Health = Health;
  SaveGameInstance->CharacterStats.MaxHealth = MaxHealth;
  SaveGameInstance->CharacterStats.Stamina = Stamina;
  SaveGameInstance->CharacterStats.MaxStamina = MaxStamina;
  SaveGameInstance->CharacterStats.Coins = Coins;
  //We dont save our position when we transition through a portal, it will be wrong
  if (bSavePosition && !bTransitioningFromLevelPortal){
    SaveGameInstance->CharacterStats.Location = GetActorLocation();
    SaveGameInstance->CharacterStats.Rotation = GetActorRotation();
  }

  SaveGameInstance->CharacterStats.TransitioningFromLevelPortal = bTransitioningFromLevelPortal;

  FString MapName = LevelName.ToString();
  if (MapName.Equals("None")){
    FString ThisMapName = GetWorld()->GetMapName();
    ThisMapName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
    MapName = ThisMapName;

  }


  SaveGameInstance->CharacterStats.LevelName = MapName;


  if (EquippedWeapon){
    SaveGameInstance->CharacterStats.WeaponName = EquippedWeapon->Name;
  }

  UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->PlayerName, SaveGameInstance->UserIndex);
  

}

void AMain::LoadGame(bool SetPosition)
{
  //LoadGameNoSwitch();

  UFirstSaveGame* LoadGameInstance = Cast<UFirstSaveGame>(UGameplayStatics::CreateSaveGameObject(UFirstSaveGame::StaticClass()));
  LoadGameInstance = Cast<UFirstSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->PlayerName, LoadGameInstance->UserIndex));
  if (!LoadGameInstance) { return; }

  FString FSLevelName = LoadGameInstance->CharacterStats.LevelName;
  FString Temp = GetWorld()->GetMapName();
  Temp.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

  if ( !FSLevelName.Equals("") && !FSLevelName.Equals(Temp) ) {
    FName FNLevelName = FName(*FSLevelName);
    SwitchLevel(FNLevelName, false, false);
  }

  UFirstSaveGame* LoadGameInstance = Cast<UFirstSaveGame>(UGameplayStatics::CreateSaveGameObject(UFirstSaveGame::StaticClass()));
  LoadGameInstance = Cast<UFirstSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->PlayerName, LoadGameInstance->UserIndex));
  if (!LoadGameInstance) { return; }

  FString FSLevelName = LoadGameInstance->CharacterStats.LevelName;
  FString Temp = GetWorld()->GetMapName();
  Temp.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
  UE_LOG(LogTemp, Warning, TEXT("AMain: LoadGame,Temp %s"), *Temp);
  UE_LOG(LogTemp, Warning, TEXT("AMain: LoadGame,FSLevelName %s"), *FSLevelName);
  if ( !FSLevelName.Equals("") && !FSLevelName.Equals(Temp) ) {
    FName FNLevelName = FName(*FSLevelName);
    UE_LOG(LogTemp, Warning, TEXT("AMain: LoadGame, calling switch level"));
    SwitchLevel(FNLevelName, false, false);
  }
  UE_LOG(LogTemp, Warning, TEXT("AMain: LoadGame, after switch level, loading health and such"));
  Health = LoadGameInstance->CharacterStats.Health;
  MaxHealth = LoadGameInstance->CharacterStats.MaxHealth;
  Stamina = LoadGameInstance->CharacterStats.Stamina;
  MaxStamina = LoadGameInstance->CharacterStats.MaxStamina;
  Coins = LoadGameInstance->CharacterStats.Coins;
  bTransitioningFromLevelPortal = LoadGameInstance->CharacterStats.TransitioningFromLevelPortal;

  FString WeaponName = LoadGameInstance->CharacterStats.WeaponName;
  if (!WeaponName.Equals(TEXT(""))) {
    //EquippedWeapon = LoadGameInstance->CharacterStats.WeaponName;
    if (WeaponStorageClass) {
      AItemStorage* WeaponStorageInst = GetWorld()->SpawnActor<AItemStorage>(WeaponStorageClass);
      if (!WeaponStorageInst) { return; }
      if (!WeaponStorageInst->WeaponMap.Contains(WeaponName)) { return; }
      TSubclassOf<AWeapon> WeaponClass = WeaponStorageInst->WeaponMap[WeaponName];

      AWeapon* Weapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass);
      Weapon->Equip(this);
    }
  }
  if (SetPosition){
    UE_LOG(LogTemp, Warning, TEXT("AMain: LoadGame, after switch level, here4"));
    SetActorLocation(LoadGameInstance->CharacterStats.Location);
    SetActorRotation(LoadGameInstance->CharacterStats.Rotation);
  }  

  SetMovementStatus(EMovementStatus::EMS_Normal);
  GetMesh()->bPauseAnims = false;
  GetMesh()->bNoSkeletonUpdate = false;
}

void AMain::LoadGameNoSwitch(bool SetPosition)
{
  UFirstSaveGame* LoadGameInstance = Cast<UFirstSaveGame>(UGameplayStatics::CreateSaveGameObject(UFirstSaveGame::StaticClass()));

  LoadGameInstance = Cast<UFirstSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->PlayerName, LoadGameInstance->UserIndex));
  if (!LoadGameInstance) { return; }

  Health = LoadGameInstance->CharacterStats.Health;
  MaxHealth = LoadGameInstance->CharacterStats.MaxHealth;
  Stamina = LoadGameInstance->CharacterStats.Stamina;
  MaxStamina = LoadGameInstance->CharacterStats.MaxStamina;
  Coins = LoadGameInstance->CharacterStats.Coins;
  bTransitioningFromLevelPortal = LoadGameInstance->CharacterStats.TransitioningFromLevelPortal;

  FString WeaponName = LoadGameInstance->CharacterStats.WeaponName;
  if (!WeaponName.Equals(TEXT(""))) {
    //EquippedWeapon = LoadGameInstance->CharacterStats.WeaponName;
    if (WeaponStorageClass) {
      AItemStorage* WeaponStorageInst = GetWorld()->SpawnActor<AItemStorage>(WeaponStorageClass);
      if (!WeaponStorageInst) { return; }
      if (!WeaponStorageInst->WeaponMap.Contains(WeaponName)) { return; }
      TSubclassOf<AWeapon> WeaponClass = WeaponStorageInst->WeaponMap[WeaponName];

      AWeapon* Weapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass);
      Weapon->Equip(this);
    }
  }
  if (SetPosition) {
    SetActorLocation(LoadGameInstance->CharacterStats.Location);
    SetActorRotation(LoadGameInstance->CharacterStats.Rotation);
  }
  SetMovementStatus(EMovementStatus::EMS_Normal);
  GetMesh()->bPauseAnims = false;
  GetMesh()->bNoSkeletonUpdate = false;

}

// Called when the game starts or when spawned
void AMain::BeginPlay()
{
  UE_LOG(LogTemp, Warning, TEXT("AMain: BeginPlay1"));
	Super::BeginPlay();
  MainPlayerController = Cast<AMainPlayerController>(GetController());


  FString MapName = GetWorld()->GetMapName();
  MapName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
  FName MyMapName(MapName);
  if (!MapName.Equals("SunTemple")) {
    //If your not transition through a portal then use the saved position when you load game
    //TODO WORK ON THIS
    //bool UsePosition = !bTransitioningFromLevelPortal;
    LoadGameNoSwitch(bTransitioningFromLevelPortal);
    //AutoSaveDelegate = FTimerDelegate::CreateUObject(this, &AMain::SaveGame, MyMapName, true);
    //AutoSaveDelegate.BindUFunction(this, FName("SaveGame"), LevelName, true);
    //GetWorldTimerManager().SetTimer(AutoSaveHandle, AutoSaveDelegate, 10.0f, false, 5.0f);
    if (bTransitioningFromLevelPortal){
      //done transitioning, dont save this again as true yet
      bTransitioningFromLevelPortal = false;
      SaveGame(MyMapName, true);
    }
  }
  if (MainPlayerController) {
    MainPlayerController->GameModeOnly();
  }
}

// Called every frame
void AMain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
  if (MovementStatus == EMovementStatus::EMS_Dead) { return; }


  if (bInterpToEnemy && CombatTarget) {
    FRotator LookAtYaw = GetLookAtRotationYaw(CombatTarget->GetActorLocation());
    FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), LookAtYaw, DeltaTime, InterpSpeed); \
      SetActorRotation(InterpRotation);
  }

  if (CombatTarget) {
    CombatTargetLocation = CombatTarget->GetActorLocation();
    if (MainPlayerController) {
      MainPlayerController->EnemyLocation = CombatTargetLocation;
    }
  }


  float DeltaStamina = StaminaDrainRate * DeltaTime;

  switch (StaminaStatus)
  {
  case EStaminaStatus::ESS_Normal:
    if (!bShiftKeyDown) { 
      Stamina += DeltaStamina;
      if (Stamina > MaxStamina) {
        Stamina = MaxStamina;
      }
      SetMovementStatus(EMovementStatus::EMS_Normal);
      break;
    }
    if (!bMovingForward && !bMovingRight) { break; }
    Stamina -= DeltaStamina;
    if (Stamina <= MinSprintStamina){ 
      SetStaminaStatus(EStaminaStatus::ESS_BelowMinimum);
    }
    SetMovementStatus(EMovementStatus::EMS_Sprinting);
    break;
  case EStaminaStatus::ESS_BelowMinimum:
    if (!bShiftKeyDown) {
      Stamina += DeltaStamina;
      if (Stamina >= MinSprintStamina) {
        SetStaminaStatus(EStaminaStatus::ESS_Normal);
      }
      SetMovementStatus(EMovementStatus::EMS_Normal);
      break;
    }
    if (!bMovingForward && !bMovingRight) { break; }
    Stamina -= DeltaStamina;
    if (Stamina <= 0.0f) {
      SetStaminaStatus(EStaminaStatus::ESS_Exhausted);
      Stamina = 0.0f;
      SetMovementStatus(EMovementStatus::EMS_Normal);
    }
    SetMovementStatus(EMovementStatus::EMS_Sprinting);
    break;
  case EStaminaStatus::ESS_Exhausted:
    if (!bShiftKeyDown) {
      SetStaminaStatus(EStaminaStatus::ESS_ExhaustedRecovering);
      Stamina += DeltaStamina;
      break;
    }
    Stamina = 0.0f;
    SetMovementStatus(EMovementStatus::EMS_Normal);
    break;
  case EStaminaStatus::ESS_ExhaustedRecovering:
    Stamina += DeltaStamina;
    if (Stamina >= MinSprintStamina) {
      SetStaminaStatus(EStaminaStatus::ESS_Normal);
    }
    SetMovementStatus(EMovementStatus::EMS_Normal);
    break;
  case EStaminaStatus::ESS_MAX:
    break;
  default:
    break;
  }





}

// Called to bind functionality to input
void AMain::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
  check(PlayerInputComponent);
  PlayerInputComponent->BindAxis("MoveForward", this, &AMain::MoveForward);
  PlayerInputComponent->BindAxis("MoveRight", this, &AMain::MoveRight);

  PlayerInputComponent->BindAxis("Turn", this, &AMain::Turn);
  PlayerInputComponent->BindAxis("LookUp", this, &AMain::Lookup);

  PlayerInputComponent->BindAxis("TurnRate", this, &AMain::TurnAtRate);
  PlayerInputComponent->BindAxis("LookUpRate", this, &AMain::LookUpAtRate);

  PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMain::Jump);
  PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);


  PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMain::ShiftKeyDown);
  PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMain::ShiftKeyUp);

  PlayerInputComponent->BindAction("LMB", IE_Pressed, this, &AMain::LMBDown);
  PlayerInputComponent->BindAction("LMB", IE_Released, this, &AMain::LMBUp);

  PlayerInputComponent->BindAction("ESC", IE_Pressed, this, &AMain::ESCDown);
  PlayerInputComponent->BindAction("ESC", IE_Released, this, &AMain::ESCUp);


}


void AMain::LMBDown()
{
  bLMBDown = true;
  if (MovementStatus == EMovementStatus::EMS_Dead) { return; }

  if (MainPlayerController) {
    if (MainPlayerController->bPauseMenuVisible){ return; }
  }


  if (ActiveOverlappingItem){ 
    AWeapon* Weapon = Cast<AWeapon>(ActiveOverlappingItem); 
    if (!Weapon){ return; }
    Weapon->Equip(this);
  }
  else if (EquippedWeapon) {
    Attack();
  }
}

void AMain::LMBUp()
{
  bLMBDown = false;
  SetActiveOverlappingItem(nullptr);
}




void AMain::MoveForward(float Value)
{

  bMovingForward = false;
  if (!CanMove(Value)) { return; }
  bMovingForward = true;

  // Find out which way is forward
  const FRotator Rotation = Controller->GetControlRotation();
  const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

  const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
  AddMovementInput(Direction, Value);

}

void AMain::MoveRight(float Value)
{
  bMovingRight = false;
  if (!CanMove(Value)){ return; }
  bMovingRight = true;

  // Find out which way is forward
  const FRotator Rotation = Controller->GetControlRotation();
  const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

  const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
  AddMovementInput(Direction, Value);


}

bool AMain::CanMove(float Value)
{

  if (MovementStatus == EMovementStatus::EMS_Dead) { return false; }

  if (MainPlayerController == nullptr || Value == 0.0f || bAttacking) {
    return false;
  }

  if (MainPlayerController->bPauseMenuVisible){ return false; }

  return true;
}

void AMain::Turn(float Value)
{
  if (!CanMove(Value)) { return; }
  AddControllerYawInput(Value);
}

void AMain::Lookup(float Value)
{
  if (!CanMove(Value)) { return; }
  AddControllerPitchInput(Value);
}

void AMain::TurnAtRate(float Rate)
{
  AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMain::LookUpAtRate(float Rate)
{
  AddControllerPitchInput(Rate * BaseLookUpAtRate * GetWorld()->GetDeltaSeconds());
}
