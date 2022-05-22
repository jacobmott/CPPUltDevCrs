// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

class UUserWidget;

/**
 * 
 */
UCLASS()
class CPPULTDEVCRS_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
  // Sets default values
  AMainPlayerController();

  /** Reference to the UMG asset in the editor */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
  TSubclassOf<UUserWidget> HUDOverlayAsset;

  /** Variable to hold the widget after we create it */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Particles")
  TObjectPtr<UUserWidget> HUDOverlay;

protected:

  virtual void BeginPlay() override;



};
