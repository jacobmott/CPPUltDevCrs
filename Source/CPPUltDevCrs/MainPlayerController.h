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
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
  TObjectPtr<UUserWidget> HUDOverlay;


  /** Reference to the UMG asset in the editor */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
  TSubclassOf<UUserWidget> EnemyHealthBarAsset;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
  TObjectPtr<UUserWidget> EnemyHealthBar;


  /** Reference to the UMG asset in the editor */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
  TSubclassOf<UUserWidget> WPauseMenu;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
  TObjectPtr<UUserWidget> PauseMenu;


  bool bEnemyHealthBarVisible;

  void DisplayEnemyHealthBar();

  void RemoveEnemyHealthBar();



  bool bPauseMenuVisible;

  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
  void DisplayPauseMenu();
  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
  void RemovePauseMenu();

  void TogglePauseMenu();

  //UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widgets")
  FVector EnemyLocation;

protected:

  virtual void BeginPlay() override;

  virtual void Tick(float DeltaTime) override;

};
