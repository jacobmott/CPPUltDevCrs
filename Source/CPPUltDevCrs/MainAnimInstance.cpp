// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAnimInstance.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Main.h"



void UMainAnimInstance::NativeInitializeAnimation()
{
  if (!Pawn) {
    Pawn = TryGetPawnOwner();
  }

  if (!Pawn) {
    return;
  }

  if (!Main) {
    Main = Cast<AMain>(Pawn);
  }

  if (!Main) {
    return;
  }

}

void UMainAnimInstance::UpdateAnimationProperties()
{
  if (!Pawn) {
    Pawn = TryGetPawnOwner();
  }

  if (!Pawn) {
    return;
  }

  if (!Main) {
    Main = Cast<AMain>(Pawn);
  }

  if (!Main) {
    return;
  }

  FVector Speed = Pawn->GetVelocity();

  FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.0f);

  //get magnitude ( x y plane, dont care about vertical)
  MovementSpeed = LateralSpeed.Size();

  bIsInAir = Pawn->GetMovementComponent()->IsFalling();

  

}
