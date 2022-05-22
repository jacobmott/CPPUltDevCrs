// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Main.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class CPPULTDEVCRS_API AMain : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMain();

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UCameraComponent> CameraComponent;
  FORCEINLINE UCameraComponent* GetCameraComponent() {
    return CameraComponent;
  }
  FORCEINLINE void SetCameraComponent(UCameraComponent* CameraComp) {
    CameraComponent = CameraComp;
  }

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta= (AllowPrivateAccess = "true"))
  TObjectPtr<USpringArmComponent> SpringArmComponent;
  FORCEINLINE USpringArmComponent* GetSpringArmComponent() {
    return SpringArmComponent;
  }
  FORCEINLINE void SetSpringArmComponent(USpringArmComponent* SpringArmComp) {
    SpringArmComponent = SpringArmComp;
  }

  // Scale turning functions for the camera
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
  float BaseTurnRate;


  // Scale turning functions for the camera
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
  float BaseLookUpAtRate;




  /**
  /* 
  /*
  /*Player Stats
  /*
  */

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerStats")
  float MaxHealth;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStats")
  float Health; 

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerStats")
  float MaxStamina;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStats")
  float Stamina;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerStats")
  int32 Coins;




protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


  //Called for forward and backward input
  void MoveForward(float Value);

  //Called for right and left input
  void MoveRight(float Value);

  /* Called via input to turn at a specific rate
  *  @param Rate This is a normalized rate, i.e. 1.0 means 100% of desired turn rate  
  */
  void TurnAtRate(float Rate);


  /* Called via input to look up/down at a specific rate
  *  @param Rate This is a normalized rate, i.e. 1.0 means 100% of desired loop up/down rate
  */
  void LookUpAtRate(float Rate);



	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
