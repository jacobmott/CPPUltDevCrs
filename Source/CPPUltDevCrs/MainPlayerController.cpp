// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"

#include "Blueprint/UserWidget.h"

AMainPlayerController::AMainPlayerController()
{
  bEnemyHealthBarVisible = false;
}

void AMainPlayerController::DisplayEnemyHealthBar()
{
  if (!EnemyHealthBar) { return; }
  EnemyHealthBar->SetVisibility(ESlateVisibility::Visible);
  bEnemyHealthBarVisible = true;
}

void AMainPlayerController::RemoveEnemyHealthBar()
{
  if (!EnemyHealthBar){ return; }
  EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
  bEnemyHealthBarVisible = false;
}

void AMainPlayerController::BeginPlay()
{
  Super::BeginPlay();


  if (HUDOverlayAsset) {
  
    HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayAsset);

    HUDOverlay->AddToViewport();

    HUDOverlay->SetVisibility(ESlateVisibility::Visible);
  }


  if (EnemyHealthBarAsset) {

    EnemyHealthBar = CreateWidget<UUserWidget>(this, EnemyHealthBarAsset);
    
    EnemyHealthBar->AddToViewport();
    
    EnemyHealthBar->SetVisibility(ESlateVisibility::Visible);

    FVector2D Alignment(0.0f, 0.0f);
    EnemyHealthBar->SetAlignmentInViewport(Alignment);
  }


}

void AMainPlayerController::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
  if (EnemyHealthBar && bEnemyHealthBarVisible) {
    FVector2D PositionInViewport;
    ProjectWorldLocationToScreen(EnemyLocation, PositionInViewport);
    PositionInViewport.Y -= 85.0f;

    FVector2D SizeInViewport = FVector2D(300.0f, 25.0f);
    EnemyHealthBar->SetPositionInViewport(PositionInViewport);
    EnemyHealthBar->SetDesiredSizeInViewport(SizeInViewport);
  }
}
