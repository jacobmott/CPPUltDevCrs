// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingPlatform.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"

// Sets default values
AFloatingPlatform::AFloatingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

  Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
  //Mesh->SetupAttachment(GetRootComponent());
  RootComponent = Mesh;

  StartPoint = FVector(0.0f, 0.0f, 0.0f);
  EndPoint = FVector(0.0f, 0.0f, 0.0f);


  InterpSpeed = 0.2f;

  AreInterping = false;

  InterpTime = 2.0f;

}

// Called when the game starts or when spawned
void AFloatingPlatform::BeginPlay()
{
	Super::BeginPlay();
	StartPoint = GetActorLocation();
  EndPoint += StartPoint;


  GetWorldTimerManager().SetTimer(InterpTimer, this, &AFloatingPlatform::ToggleInterping, InterpTime);

  TotalDistance = (EndPoint - StartPoint).Size();

}

// Called every frame
void AFloatingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
  if (!AreInterping){ return; }


  FVector CurrentLocation = GetActorLocation();
  FVector Interp = FMath::VInterpTo(CurrentLocation, EndPoint, DeltaTime, InterpSpeed);

  SetActorLocation(Interp);
  float DistanceTraveled = (GetActorLocation() - StartPoint).Size();

  if (TotalDistance - DistanceTraveled >= 1.0f) { return; }


  ToggleInterping();

  SwapVectors(StartPoint, EndPoint);
  GetWorldTimerManager().SetTimer(InterpTimer, this, &AFloatingPlatform::ToggleInterping, InterpTime);


}

void AFloatingPlatform::ToggleInterping()
{
  AreInterping = !AreInterping;
}

void AFloatingPlatform::SwapVectors(FVector& VecOne, FVector& VecTwo)
{
  FVector VecOneTemp = VecOne;
  VecOne = VecTwo;
  VecTwo = VecOneTemp;

}

