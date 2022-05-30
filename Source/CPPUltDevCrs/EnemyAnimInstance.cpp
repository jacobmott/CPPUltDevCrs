// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"

#include "Enemy.h"


void UEnemyAnimInstance::NativeInitializeAnimation()
{
  if (!Pawn) {
    Pawn = TryGetPawnOwner();
  }

  if (!Pawn) {
    return;
  }

  if (!Enemy) {
    Enemy = Cast<AEnemy>(Pawn);
  }

  if (!Enemy) {
    return;
  }

}

void UEnemyAnimInstance::UpdateAnimationProperties()
{
  if (!Pawn) {
    Pawn = TryGetPawnOwner();
  }

  if (!Pawn) {
    return;
  }

  if (!Enemy) {
    Enemy = Cast<AEnemy>(Pawn);
  }

  if (!Enemy) {
    return;
  }

  FVector Speed = Pawn->GetVelocity();

  FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.0f);

  //get magnitude ( x y plane, dont care about vertical)
  MovementSpeed = LateralSpeed.Size();

  //bIsInAir = Pawn->GetMovementComponent()->IsFalling();



}