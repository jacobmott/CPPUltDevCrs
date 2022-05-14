// Fill out your copyright notice in the Description page of Project Settings.


#include "Critter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"

// Sets default values
ACritter::ACritter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
  SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
  SkeletalMeshComponent->SetupAttachment(RootComponent);

  CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(GetRootComponent());
  CameraComponent->SetRelativeLocation(FVector(-300.0f, 0.0f, 300.0f));
	CameraComponent->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
  //SetRootComponent(StaticMeshComponent);
 
  //We dont possess this, its a monster/critter
  //AutoPossessPlayer = EAutoReceiveInput::Player0;


  CurrentVelocity = FVector(0.0f, 0.0f, 0.0f);
  MaxSpeed = 100.0f;

}

// Called when the game starts or when spawned
void ACritter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACritter::MoveForward(float Value)
{


  CurrentVelocity.X = FMath::Clamp(Value, -1.0f, 1.0f) * MaxSpeed;
  //if (Controller && (Value != 0.0f)) {
  //  const FRotator Rotation = Controller->GetControlRotation();
  //  const FRotator YawRotation(0, Rotation.Yaw, 0);
  //  const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
  //  AddMovementInput(Direction, Value);
  //}
}

void ACritter::MoveRight(float Value)
{


  CurrentVelocity.Y = FMath::Clamp(Value, -1.0f, 1.0f) * MaxSpeed;
  //if (Controller && (Value != 0.0f)) {
  //  const FRotator Rotation = Controller->GetControlRotation();
  //  const FRotator YawRotation(0, Rotation.Yaw, 0);
  //  const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
  //  AddMovementInput(Direction, Value);
  //}
}




// Called every frame
void ACritter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
 
  FVector NewLocation = GetActorLocation() + (CurrentVelocity * DeltaTime);
  SetActorLocation(NewLocation);

}

// Called to bind functionality to input
void ACritter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

  PlayerInputComponent->BindAxis("MoveForward", this, &ACritter::MoveForward);
  PlayerInputComponent->BindAxis("MoveRight", this, &ACritter::MoveRight);

}

