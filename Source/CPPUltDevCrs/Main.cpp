// Fill out your copyright notice in the Description page of Project Settings.


#include "Main.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AMain::AMain()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

  SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
  SpringArmComponent->SetupAttachment(GetRootComponent());
  SpringArmComponent->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
  SpringArmComponent->TargetArmLength = 600.0f;
  SpringArmComponent->bUsePawnControlRotation = true; // Rotate arm based on controller
  SpringArmComponent->bEnableCameraLag = true;
  SpringArmComponent->CameraLagSpeed = 3.0f;

  // Pulls toward player if theres a collision
  CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
  CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
  // Attach the camera to end of spring arm and let the spring arm adjust to match the controller oreientation
  CameraComponent->bUsePawnControlRotation = false; 

  GetCapsuleComponent()->SetCapsuleSize(50.0f, 105.0f);

  // Set our turn rates for input
  BaseTurnRate = 65.0f;
  BaseLookUpAtRate = 65.0f;

  //We modify the controller with the camera when we turn our mouse, but we dont want the
  //characters movement/rotation to also rotate when we rotate the camera
  //So, Don't rotate when the controller rotates
  //Let that just affect the camera
  bUseControllerRotationYaw = false;
  bUseControllerRotationPitch = false;
  bUseControllerRotationRoll = false;


  //Configure character movement
  //Character moves in the direction of input(left or right move)
  GetCharacterMovement()->bOrientRotationToMovement = true;
  GetCharacterMovement()->RotationRate = FRotator(0.0f, 640.0f, 0.0f); //At this rotation rate
  GetCharacterMovement()->JumpZVelocity = 650.0f;
  GetCharacterMovement()->AirControl = 0.4f;
}

// Called when the game starts or when spawned
void AMain::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMain::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
  check(PlayerInputComponent);
  PlayerInputComponent->BindAxis("MoveForward", this, &AMain::MoveForward);
  PlayerInputComponent->BindAxis("MoveRight", this, &AMain::MoveRight);

  PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
  PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

  PlayerInputComponent->BindAxis("TurnRate", this, &AMain::TurnAtRate);
  PlayerInputComponent->BindAxis("LookUpRate", this, &AMain::LookUpAtRate);

  PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
  PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
}



void AMain::MoveForward(float Value)
{
  if (Controller == nullptr  || Value == 0.0f){
    return;
  }

  // Find out which way is forward
  const FRotator Rotation = Controller->GetControlRotation();
  const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

  const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
  AddMovementInput(Direction, Value);

}

void AMain::MoveRight(float Value)
{

  if (Controller == nullptr || Value == 0.0f) {
    return;
  }

  // Find out which way is forward
  const FRotator Rotation = Controller->GetControlRotation();
  const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

  const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
  AddMovementInput(Direction, Value);


}

void AMain::TurnAtRate(float Rate)
{
  AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMain::LookUpAtRate(float Rate)
{
  AddControllerPitchInput(Rate * BaseLookUpAtRate * GetWorld()->GetDeltaSeconds());
}