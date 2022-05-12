// Fill out your copyright notice in the Description page of Project Settings.


#include "Collider.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/SphereComponent.h"

#include "GameFramework/SpringArmComponent.h"

#include "UObject/ConstructorHelpers.h"

// Sets default values
ACollider::ACollider()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

  RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
  SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
  SphereComponent->SetupAttachment(GetRootComponent());
  SphereComponent->InitSphereRadius(40.0f);
  SphereComponent->SetCollisionProfileName(TEXT("Pawn"));

  StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
  StaticMeshComponent->SetupAttachment(GetRootComponent());
  static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshComponentAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
  if (MeshComponentAsset.Succeeded()) {
    StaticMeshComponent->SetStaticMesh(MeshComponentAsset.Object);
    StaticMeshComponent->SetRelativeLocation(FVector(0.0f,0.0f,-40.0f));
    StaticMeshComponent->SetWorldScale3D(FVector(0.8f, 0.8f, 0.8f));
  }




  SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
  SpringArmComponent->SetupAttachment(GetRootComponent());
  SpringArmComponent->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
  SpringArmComponent->TargetArmLength = 400.0f;
  SpringArmComponent->bEnableCameraLag = true;
  SpringArmComponent->CameraLagSpeed = 3.0f;

  CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
  CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);

  AutoPossessPlayer = EAutoReceiveInput::Player0;

}

// Called when the game starts or when spawned
void ACollider::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACollider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ACollider::MoveForward(float Value)
{

  FVector Forward = GetActorForwardVector();
  AddMovementInput(Forward, Value);
}

void ACollider::MoveRight(float Value)
{


  FVector Right = GetActorRightVector();
  AddMovementInput(Right, Value);
}


// Called to bind functionality to input
void ACollider::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

  PlayerInputComponent->BindAxis("MoveForward", this, &ACollider::MoveForward);
  PlayerInputComponent->BindAxis("MoveRight", this, &ACollider::MoveRight);

}

