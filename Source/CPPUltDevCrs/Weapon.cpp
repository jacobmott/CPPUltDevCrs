// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Main.h"
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




}

void AWeapon::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
  Super::OnOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
  UE_LOG(LogTemp, Warning, TEXT("AWeapon: Overlap Begin"));

  if (!OtherActor){ return; }

  AMain* Main = Cast<AMain>(OtherActor);
  if (!Main){ return; }
  Equip(Main);

}

void AWeapon::OnOverlapEnd(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
  Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
  UE_LOG(LogTemp, Warning, TEXT("AWeapon: Overlap End"));
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

  bRotate = false;

}
