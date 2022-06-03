// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"
#include "Main.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/World.h"
#include "Sound/SoundCue.h"


APickup::APickup()
{
  //CountCount = 1;
}

void APickup::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
  Super::OnOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
  UE_LOG(LogTemp, Warning, TEXT("APickup: Overlap Begin"));


  if (!OtherActor) {
    return;
  }

  AMain* Main = Cast<AMain>(OtherActor);
  if (!Main) {
    return;
  }
  OnPickupBP(Main);

  //Main->IncrementCoins(CountCount);
  Main->PickupLocations.Add(GetActorLocation());

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

void APickup::OnOverlapEnd(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
  Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
  UE_LOG(LogTemp, Warning, TEXT("APickup: Overlap End"));
  //GetWorldTimerManager().SetTimer(SwitchHandle, this, &AFloorSwitch::CloseDoor, SwitchTime);
}