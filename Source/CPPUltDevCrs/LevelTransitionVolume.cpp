// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelTransitionVolume.h"

#include "Components/BoxComponent.h"

#include "Components/BillboardComponent.h"

#include "Main.h"


// Sets default values
ALevelTransitionVolume::ALevelTransitionVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

  TransitionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TransitionVolume"));
  RootComponent = TransitionVolume;

  BillboardComp = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
  BillboardComp->SetupAttachment(GetRootComponent());

  TransitionLevelName = "SunTemple";


}

// Called when the game starts or when spawned
void ALevelTransitionVolume::BeginPlay()
{
	Super::BeginPlay();
  TransitionVolume->OnComponentBeginOverlap.AddDynamic(this, &ALevelTransitionVolume::OnOverlapBegin);
  //OnActorBeginOverlap.AddDynamic(this, &ALevelTransitionVolume::OnOverlapBegin);
}

// Called every frame
void ALevelTransitionVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelTransitionVolume::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
  AMain* Main = Cast<AMain>(OtherActor);
  if (!Main){ return; }
  Main->bTransitioningFromLevelPortal = true;
  Main->SwitchLevel(TransitionLevelName, true, false);

}

void ALevelTransitionVolume::OnOverlapEnd(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
  AMain* Main = Cast<AMain>(OtherActor);
  if (!Main) { return; }

}

