// Fill out your copyright notice in the Description page of Project Settings.


#include "Floater.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AFloater::AFloater()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

  StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Custom Static Mesh"));


  InitialLocation = FVector(0.0f);
  PlacedLocation = FVector(0.0f);
  WorldOrigin = FVector(0.0f, 0.0f, 0.0f);

  InitialDirection = FVector(0.0f, 0.0f, 0.0f);

  bInitializedFloaterLocations = false;
  bShouldFloat = false;


}

// Called when the game starts or when spawned
void AFloater::BeginPlay()
{
	Super::BeginPlay();

  PlacedLocation = GetActorLocation();


  if(bInitializedFloaterLocations){
    SetActorLocation(InitialLocation);
  }

}

// Called every frame
void AFloater::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

  if (bShouldFloat) {
    FHitResult HitResult;
    AddActorLocalOffset(InitialDirection, true, &HitResult);
    FVector HitLocation = HitResult.Location;
    //UE_LOG(LogTemp, Warning, TEXT("Hit Location: X = %f, Y = %f, Z = %f"), HitLocation.X, HitLocation.Y, HitLocation.Z);
  }

}

