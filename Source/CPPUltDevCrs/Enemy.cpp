// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"

#include "Particles/ParticleSystemComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

#include "AIController.h"

#include "Main.h"

#include "Components/SkeletalMeshComponent.h"

#include "Engine/SkeletalMeshSocket.h"

#include "Sound/SoundCue.h"

#include "Animation/AnimInstance.h"

#include "TimerManager.h"

#include "Components/CapsuleComponent.h"

#include "MainPlayerController.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;


  AgroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AgroSphere"));
  AgroSphere->SetupAttachment(GetRootComponent());
  AgroSphere->InitSphereRadius(600.0f);
  AgroSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECR_Ignore);


  CombatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CombatSphere"));
  CombatSphere->SetupAttachment(GetRootComponent());
  CombatSphere->InitSphereRadius(75.0f);
  CombatSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
  CombatSphere->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
  CombatSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
  CombatSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Overlap);
  //CombatSphere->MoveIgnoreActors.Add(this);

  CombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CombatCollision"));
  //CombatCollision->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("EnemySocket"));
  CombatCollision->SetupAttachment(GetMesh(), FName("EnemySocket"));
  CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  CombatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
  CombatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
  CombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Overlap);

  //const USkeletalMeshSocket* LeftKneeeSocket = GetMesh()->GetSocketByName("EnemySocket");
  //if (LeftKneeeSocket) { 
  //  RightHandSocket->AttachActor(this, Char->GetMesh());
  //}

  bOverlappingCombatSphere = false;


  Health = 75.0f;
  MaxHealth = 100.0f;
  Damage = 10.0f;
  bAttacking = false;

  AttackMinTime = 0.5f;
  AttackMaxTime = 3.5f;

  EnemyMovementStatus = EEnemyMovementStatus::EMS_Idle;

  DeathDelay = 3.0f;
  bHasValidTarget = false;

}

float AEnemy::TakeDamage(float DamageAmout, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{

  Health -= DamageAmout;
  if (Health <= 0.0f) {
    Health = 0.0f;
    Die(DamageCauser);
  }
  return DamageAmout;
}

void AEnemy::Die(AActor* Causer)
{
  SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Dead);

  UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
  if (AnimInstance && CombatMontage) {
    AnimInstance->Montage_Play(CombatMontage, 1.0f);
    AnimInstance->Montage_JumpToSection(FName("Death"), CombatMontage);
  }

  CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  AgroSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  CombatSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

  if (!Causer){return;}
  AMain* Main = Cast<AMain>(Causer);
  if (!Main){return;}
  Main->MainPlayerController->RemoveEnemyHealthBar();
  Main->UpdateCombatTarget();
  
}

void AEnemy::DeathEnd()
{
  //UE_LOG(LogTemp, Warning, TEXT("AEnemy: DeathEnd"));
  GetMesh()->bPauseAnims = true;
  GetMesh()->bNoSkeletonUpdate = true;
  GetWorldTimerManager().SetTimer(DeathTimer, this, &AEnemy::Dissappear, DeathDelay);
}

bool AEnemy::Alive()
{
  return GetEnemyMovementStatus() != EEnemyMovementStatus::EMS_Dead;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
  AIController = Cast<AAIController>(GetController());

  AgroSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::AgroSphereOnOverlapBegin);
  AgroSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::AgroSphereOnOverlapEnd);

  CombatSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapBegin);
  CombatSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapEnd);

  CombatCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatOnOverlapBegin);
  CombatCollision->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatOnOverlapEnd);

  //Dont make the camera zoom in when its in a weird spot where the enemies mesh is in between it and the player  
  GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
  GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}




void AEnemy::AgroSphereOnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

  if (!Alive()){ return; }
  if (!OtherActor){ return; }
  if (OtherActor == this){ return; }

  //UE_LOG(LogTemp, Warning, TEXT("AEnemy: AgroSphereOnOverlapBegin"));

  AMain* Main = Cast<AMain>(OtherActor);
  if (!Main) { return; }
  MoveToTarget(Main);
  //if (!AIController){ return; }

  //AIController->MoveTo()

  //if (!OverlapParticles) {
  //  return;
  //}
  //UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OverlapParticles, GetActorLocation(), FRotator(0.0f), true);
  //
  //if (!OverlapSound) {
  //  return;
  //}
  //UGameplayStatics::PlaySound2D(this, OverlapSound);


}

void AEnemy::AgroSphereOnOverlapEnd(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

  if (!Alive()) { return; }
  if (!OtherActor) { return; }
  if (!AIController) { return; }
  //UE_LOG(LogTemp, Warning, TEXT("AEnemy: AgroSphereOnOverlapEnd"));

  AMain* Main = Cast<AMain>(OtherActor);
  if (!Main) { return; }
  //if (Main->MainPlayerController) {
    //Main->MainPlayerController->RemoveEnemyHealthBar();
  Main->UpdateCombatTarget();
  //}
  if (Main->CombatTarget == this) {
    Main->SetCombatTarget(nullptr);
    Main->SetHasCombatTarget(false);
    bHasValidTarget = false;
  }
  SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);
  AIController->StopMovement();


  //GetWorldTimerManager().SetTimer(SwitchHandle, this, &AFloorSwitch::CloseDoor, SwitchTime);
}



void AEnemy::CombatSphereOnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
  //UE_LOG(LogTemp, Warning, TEXT("AEnemy: CombatSphereOnOverlapBegin TOP: %s"), *OtherActor->GetName());
  
  if (!Alive()) { return; }
  if (!OtherActor) { return; }
  if (OtherActor == this) { return; }

  //UE_LOG(LogTemp, Warning, TEXT("AEnemy: CombatSphereOnOverlapBegin"));

  AMain* Main = Cast<AMain>(OtherActor);
  if (!Main) { return; }
  bHasValidTarget = true;
  CombatTarget = Main;
  bOverlappingCombatSphere = true;
  Main->SetCombatTarget(this);
  Main->SetHasCombatTarget(true);
  //if (Main->MainPlayerController) {
  //  Main->MainPlayerController->DisplayEnemyHealthBar();
  //}
  Main->UpdateCombatTarget();
  float AttackTime = FMath::FRandRange(AttackMinTime, AttackMaxTime);
  GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
  //Attack();

  //if (!OverlapParticles) {
  //  return;
  //}
  //UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OverlapParticles, GetActorLocation(), FRotator(0.0f), true);
  //
  //if (!OverlapSound) {
  //  return;
  //}
  //UGameplayStatics::PlaySound2D(this, OverlapSound);


}

void AEnemy::CombatSphereOnOverlapEnd(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
  if (!Alive()) { return; }
  if (!OtherActor) { return; }
  if (OtherActor == this) { return; }
  if (!OtherComp){ return; }

  //UE_LOG(LogTemp, Warning, TEXT("AEnemy: CombatSphereOnOverlapEnd"));

  AMain* Main = Cast<AMain>(OtherActor);
  if (!Main) { return; }
  bOverlappingCombatSphere = false;


  //if (EnemyMovementStatus == EEnemyMovementStatus::EMS_Attacking ||
  //  EnemyMovementStatus != EEnemyMovementStatus::EMS_Attacking) {
  MoveToTarget(Main);
  CombatTarget = nullptr;
  if (Main->CombatTarget == this){
    Main->SetCombatTarget(nullptr);
    Main->bHasCombatTarget = false;
    Main->UpdateCombatTarget();
  }
  if (Main->MainPlayerController) {
    USkeletalMeshComponent* MainMesh = Cast<USkeletalMeshComponent>(OtherComp);
    if (MainMesh) {
      Main->MainPlayerController->RemoveEnemyHealthBar();
    }
  }
  //}

  GetWorldTimerManager().ClearTimer(AttackTimer);
  //SetEnemyMovementStatus(EEnemyMovementStatus::EMS_MoveToTarget);
  //MoveToTarget(Main);

  //GetWorldTimerManager().SetTimer(SwitchHandle, this, &AFloorSwitch::CloseDoor, SwitchTime);
}

void AEnemy::MoveToTarget(AMain* Target)
{
  if (!Alive()) { return; }
  if (!AIController) { return; }
  if (!Target) { return; }

  //UE_LOG(LogTemp, Warning, TEXT("AEnemy: MoveToTarget"));

  SetEnemyMovementStatus(EEnemyMovementStatus::EMS_MoveToTarget);
  
  FNavPathSharedPtr OutNavPath;
  FAIMoveRequest MoveRequest;
  MoveRequest.SetGoalActor(Target);
  MoveRequest.SetAcceptanceRadius(5.0f);
  AIController->MoveTo(MoveRequest, &OutNavPath);

  //switch (EnemyMovementStatus)
  //{
  //case EEnemyMovementStatus::EMS_MoveToTarget:
  //	break;
  //}

  //auto PathPoints = OutNavPath->GetPathPoints();
  //
  //for (auto Point : PathPoints) {
  //  FVector Location = Point.Location;
  //  UKismetSystemLibrary::DrawDebugSphere(this, Location, 25.0f, 8, FLinearColor::Red, 10.0f, 1.5f); 
  //}

}

void AEnemy::AttackEnd()
{
  bAttacking = false;
  if (bOverlappingCombatSphere) {
    float AttackTime = FMath::FRandRange(AttackMinTime, AttackMaxTime);
    GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
  }
}


void AEnemy::Attack()
{

  if (!bHasValidTarget){ return; }
  if (!Alive()) { return; }
  if (bAttacking) { return; }
  if (!AIController) { return; }
  bAttacking = true;

  AIController->StopMovement();


  EnemyMovementStatus = EEnemyMovementStatus::EMS_Attacking;


  UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
  if (AnimInstance && CombatMontage) {
    AnimInstance->Montage_Play(CombatMontage, 1.35f);
    AnimInstance->Montage_JumpToSection(FName("Attack"), CombatMontage);

  }

}



void AEnemy::Dissappear()
{
  Destroy();
}

void AEnemy::CombatOnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
  if (!Alive()) { return; }
  if (!OtherActor) { return; }

  AMain* Main = Cast<AMain>(OtherActor);
  if (!Main) { return; }


  if (Main->HitParticles) {
    const USkeletalMeshSocket* TipSocket = GetMesh()->GetSocketByName("TipSocket");
    if (!TipSocket) { return; }
    FVector SocketLocation = TipSocket->GetSocketLocation(GetMesh());
    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Main->HitParticles, SocketLocation, FRotator(0.0f), false);
  }

  if (Main->HitSound) {
    UGameplayStatics::PlaySound2D(this, Main->HitSound);
  }

  if (DamageTypeClass) {
    UGameplayStatics::ApplyDamage(Main, Damage, AIController, this, DamageTypeClass);
  }


}


void AEnemy::CombatOnOverlapEnd(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
  if (!Alive()) { return; }
}



void AEnemy::ActivateCollision()
{
  CombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
  //if (!SwingSound) { return; }
  //UGameplayStatics::PlaySound2D(this, SwingSound);
  if (SwingSound) {
    UGameplayStatics::PlaySound2D(this, SwingSound);
  }

}

void AEnemy::DeactivateCollision()
{
  CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}




