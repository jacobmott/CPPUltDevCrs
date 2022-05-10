// Fill out your copyright notice in the Description page of Project Settings.


#include "Floater.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AFloater::AFloater()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

  StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Custom Static Mesh"));
  RootComponent = StaticMeshComponent;

  InitialLocation = FVector(0.0f);
  PlacedLocation = FVector(0.0f);
  WorldOrigin = FVector(0.0f, 0.0f, 0.0f);

  InitialDirection = FVector(0.0f, 0.0f, 0.0f);

  bInitializedFloaterLocations = false;
  bShouldFloat = false;

  InitialForce = FVector(2000000.0f, 0.0f, 0.0f);
  InitialTorque = FVector(2000000.0f, 0.0f, 0.0f);

  RunningTime = 0.0f; 


  A = 1.0f;
  B = 1.0f;
  C = 1.0f;


}

// Called when the game starts or when spawned
void AFloater::BeginPlay()
{
	Super::BeginPlay();

  //float InitialX = FMath::FRand();
  //float InitialY = FMath::FRand();
  //float InitialZ = FMath::FRand();

  float InitialX = FMath::FRandRange(-500.0f, 500.0f);
  float InitialY = FMath::FRandRange(-500.0f, 500.0f);
  float InitialZ = FMath::FRandRange(0.0f, 500.0f);

  InitialLocation.X = InitialX;
  InitialLocation.Y = InitialY;
  InitialLocation.Z = InitialZ;

  //InitialLocation *= 500;

  PlacedLocation = GetActorLocation();


  if(bInitializedFloaterLocations){
    SetActorLocation(InitialLocation);
  }

  //FVector InitialForce = FVector(2000000.0f, 0.0f, 0.0f);

  //StaticMeshComponent->AddForce(InitialForce);
  //StaticMeshComponent->AddTorqueInDegrees(InitialTorque);
  //FRotator Rotation = FRotator(0.0f, 0.0f, 30.0f);
  //AddActorLocalRotation(Rotation);

  //FHitResult HitResult;
  //FVector LocalOffset = FVector(300.0f, 0.0f, 0.0f);
  //AddActorWorldOffset(LocalOffset, true, &HitResult);
  //AddActorLocalOffset(LocalOffset, true, &HitResult);
}

// Called every frame
void AFloater::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

  if (bShouldFloat) {

    FVector NewLocation = GetActorLocation();
    
    float ScaleHowHighOrLowAmplitude = A;
    float ScaleHowFastOrSlowPeriod = B; //Frequency is the inverse of period
    //https://www.khanacademy.org/science/physics/mechanical-waves-and-sound/mechanical-waves/v/amplitude-period-frequency-and-wavelength-of-periodic-waves
    //float TranslateOverTime = C; 
    //+TranslateOverTime
    NewLocation.X = NewLocation.X + ScaleHowHighOrLowAmplitude * FMath::Sin(ScaleHowFastOrSlowPeriod * RunningTime );
    NewLocation.Y = NewLocation.Y + ScaleHowHighOrLowAmplitude * FMath::Cos(ScaleHowFastOrSlowPeriod * RunningTime);

    SetActorLocation(NewLocation);
    RunningTime += DeltaTime;
    //FHitResult HitResult;
    //AddActorLocalOffset(InitialDirection, true, &HitResult);
    //FVector HitLocation = HitResult.Location;
    //UE_LOG(LogTemp, Warning, TEXT("Hit Location: X = %f, Y = %f, Z = %f"), HitLocation.X, HitLocation.Y, HitLocation.Z);
  }

  //FRotator Rotation = FRotator(0.0f, 0.0f, 1.0f);
  //AddActorLocalRotation(Rotation);
  //AddActorWorldRotation(Rotation);
}

