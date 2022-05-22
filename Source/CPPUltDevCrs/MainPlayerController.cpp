// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"

#include "Blueprint/UserWidget.h"

AMainPlayerController::AMainPlayerController()
{

}

void AMainPlayerController::BeginPlay()
{
  Super::BeginPlay();


  if (!HUDOverlayAsset) {
    return;
  }
  
  HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayAsset);

  HUDOverlay->AddToViewport();

  HUDOverlay->SetVisibility(ESlateVisibility::Visible);
}
