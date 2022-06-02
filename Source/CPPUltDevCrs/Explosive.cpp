// Fill out your copyright notice in the Description page of Project Settings.


#include "Explosive.h"
#include "Main.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/World.h"
#include "Sound/SoundCue.h"
#include "Enemy.h"

AExplosive::AExplosive()
{
  Damage = 15.0f;
}

void AExplosive::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
  Super::OnOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
  UE_LOG(LogTemp, Warning, TEXT("AExplosive: Overlap Begin"));



  if (!OtherActor) {
    return;
  }

  AMain* Main = Cast<AMain>(OtherActor);
  AEnemy* Enemy = Cast<AEnemy>(OtherActor);
  if (!Main && !Enemy) {
    return;
  }

  if (DamageTypeClass) {
    UGameplayStatics::ApplyDamage(OtherActor, Damage, nullptr, this, DamageTypeClass);
  }

  //Main->DecrementHealth(Damage);

  if (!OverlapParticles) {
    return;
  }
  UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OverlapParticles, GetActorLocation(), FRotator(0.0f), true);

  if (!OverlapSound) {
    return;
  }
  UGameplayStatics::PlaySound2D(this, OverlapSound);



  //For item pickups.. destroy the actor/item once we've overlapped with it
  Destroy();
}

void AExplosive::OnOverlapEnd(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
  Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
  UE_LOG(LogTemp, Warning, TEXT("AExplosive: Overlap End"));
  //GetWorldTimerManager().SetTimer(SwitchHandle, this, &AFloorSwitch::CloseDoor, SwitchTime);
}