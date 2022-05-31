// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Main.h"
#include "Engine/SkeletalMeshSocket.h"

#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

#include "Particles/ParticleSystemComponent.h"

#include "Components/BoxComponent.h"

#include "Enemy.h"

#include "Engine/SkeletalMeshSocket.h"

AWeapon::AWeapon()
{
  SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
  SkeletalMeshComponent->SetupAttachment(GetRootComponent());
  if (SkeletalMeshComponent)
  {
    //Mesh->AlwaysLoadOnClient = true;
    //Mesh->AlwaysLoadOnServer = true;
    //Mesh->bOwnerNoSee = false;
    //Mesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
    //Mesh->bCastDynamicShadow = true;
    //Mesh->bAffectDynamicIndirectLighting = true;
    //Mesh->PrimaryComponentTick.TickGroup = TG_PrePhysics;
    //Mesh->SetupAttachment(CapsuleComponent);
    //static FName MeshCollisionProfileName(TEXT("CharacterMesh"));
    //Mesh->SetCollisionProfileName(MeshCollisionProfileName);
    //Mesh->SetGenerateOverlapEvents(false);
    //Mesh->SetCanEverAffectNavigation(false);
  }

  CombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CombatCollision"));
  CombatCollision->SetupAttachment(GetRootComponent());
  CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  CombatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
  CombatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
  CombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Overlap);
  //CombatCollision->MoveIgnoreActors.Add(this);

  bWeaponParticles = false;
  WeaponState = EWeaponState::EWS_Pickup;

  Damage = 25.0f;

}

void AWeapon::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
  Super::OnOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
  UE_LOG(LogTemp, Warning, TEXT("AWeapon: Overlap Begin"));

  if ( WeaponState != EWeaponState::EWS_Pickup ){ return; }
  if (!OtherActor){ return; }

  AMain* Main = Cast<AMain>(OtherActor);
  if (!Main){ return; }


  Main->SetActiveOverlappingItem(this);


  //Equip(Main);

}

void AWeapon::OnOverlapEnd(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
  Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
  UE_LOG(LogTemp, Warning, TEXT("AWeapon: Overlap End"));

  if (!OtherActor) { return; }

  AMain* Main = Cast<AMain>(OtherActor);
  if (!Main) { return; }

  Main->SetActiveOverlappingItem(nullptr);
}


void AWeapon::CombatOnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
  if (!OtherActor){ return; }
  
  AEnemy* Enemy = Cast<AEnemy>(OtherActor);
  if (!Enemy) { return; }

  if (Enemy->HitParticles) {
    const USkeletalMeshSocket* WeaponSocket = SkeletalMeshComponent->GetSocketByName("WeaponSocket");
    if (!WeaponSocket) { return; }
    FVector SocketLocation = WeaponSocket->GetSocketLocation(SkeletalMeshComponent);
    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Enemy->HitParticles, SocketLocation, FRotator(0.0f), false);
  }
  
  if (Enemy->HitSound) {
    UGameplayStatics::PlaySound2D(this, Enemy->HitSound);
  }


}


void AWeapon::CombatOnOverlapEnd(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void AWeapon::Equip(AMain* Char)
{
  if (!Char){ return; }
  
  SkeletalMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
  SkeletalMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
  SkeletalMeshComponent->SetSimulatePhysics(false);
  const USkeletalMeshSocket* RightHandSocket = Char->GetMesh()->GetSocketByName("RightHandSocket");
  if (!RightHandSocket){ return; }
  RightHandSocket->AttachActor(this, Char->GetMesh());


  Char->SetEquippedWeapon(this);
  Char->SetActiveOverlappingItem(nullptr);

  bRotate = false;
  if (OnEquipSound){
    UGameplayStatics::PlaySound2D(this, OnEquipSound);
  }
  if (!bWeaponParticles) {
    IdleParticlesComponent->Deactivate();
  }

}

void AWeapon::ActivateCollision()
{ 
  CombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
  //if (!SwingSound) { return; }
  //UGameplayStatics::PlaySound2D(this, SwingSound);
}

void AWeapon::DeactivateCollision()
{
  CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeapon::BeginPlay()
{
  Super::BeginPlay();
  CombatCollision->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::CombatOnOverlapBegin);
  CombatCollision->OnComponentEndOverlap.AddDynamic(this, &AWeapon::CombatOnOverlapEnd);
}
