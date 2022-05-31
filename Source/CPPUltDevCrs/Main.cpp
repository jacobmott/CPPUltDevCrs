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

#include "Weapon.h"
#include "Sound/SoundCue.h"

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

  if (bAttacking){ return; }
  bAttacking = true;

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
  
}

void AMain::IncrementCoins(int32 Amount)
{
  Coins += Amount;
}

// Called when the game starts or when spawned
void AMain::BeginPlay()
{
	Super::BeginPlay();

  
}

// Called every frame
void AMain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


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

  PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
  PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

  PlayerInputComponent->BindAxis("TurnRate", this, &AMain::TurnAtRate);
  PlayerInputComponent->BindAxis("LookUpRate", this, &AMain::LookUpAtRate);

  PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
  PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);


  PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMain::ShiftKeyDown);
  PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMain::ShiftKeyUp);

  PlayerInputComponent->BindAction("LMB", IE_Pressed, this, &AMain::LMBDown);
  PlayerInputComponent->BindAction("LMB", IE_Released, this, &AMain::LMBUp);

}


void AMain::LMBDown()
{
  bLMBDown = true;
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
  if (Controller == nullptr  || Value == 0.0f || bAttacking){
    return;
  }

  // Find out which way is forward
  const FRotator Rotation = Controller->GetControlRotation();
  const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

  const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
  AddMovementInput(Direction, Value);

}

void AMain::MoveRight(float Value)
{

  if (Controller == nullptr || Value == 0.0f || bAttacking) {
    return;
  }

  // Find out which way is forward
  const FRotator Rotation = Controller->GetControlRotation();
  const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

  const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
  AddMovementInput(Direction, Value);


}

void AMain::TurnAtRate(float Rate)
{
  AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMain::LookUpAtRate(float Rate)
{
  AddControllerPitchInput(Rate * BaseLookUpAtRate * GetWorld()->GetDeltaSeconds());
}
