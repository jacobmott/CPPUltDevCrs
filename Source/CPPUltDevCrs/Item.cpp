// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Particles/ParticleSystemComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

#include "Sound/SoundCue.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	CollisionVolume = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionVolume"));
  RootComponent = CollisionVolume;

  StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
  StaticMeshComponent->SetupAttachment(GetRootComponent());


  IdleParticlesComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("IdleParticlesComponent"));
  IdleParticlesComponent->SetupAttachment(GetRootComponent());

  bRotate = false;
  RotationRate = 45.0f;

}


void AItem::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
  UE_LOG(LogTemp, Warning, TEXT("AItem: Overlap Begin"));

  //For item pickups.. destory the actor/item once we've overlapped with it
  Destroy();

  if (!OverlapParticles) {
    return;
  }
  UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),OverlapParticles, GetActorLocation(), FRotator(0.0f), true );
  
  if (!OverlapSound) {
    return;
  }
  UGameplayStatics::PlaySound2D(this, OverlapSound);


}

void AItem::OnOverlapEnd(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

  UE_LOG(LogTemp, Warning, TEXT("AItem: Overlap End"));
  //GetWorldTimerManager().SetTimer(SwitchHandle, this, &AFloorSwitch::CloseDoor, SwitchTime);
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
  CollisionVolume->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnOverlapBegin);
  CollisionVolume->OnComponentEndOverlap.AddDynamic(this, &AItem::OnOverlapEnd);
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

  if (!bRotate) {
    return;
  }

  FRotator Rotation = GetActorRotation();
  Rotation.Yaw += DeltaTime * RotationRate;
  SetActorRotation(Rotation);

}

