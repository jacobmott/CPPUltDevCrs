// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Main.generated.h"

class USpringArmComponent;
class UCameraComponent;
class AWeapon;
class AItem;
class UAnimMontage;
class UParticleSystem;
class USoundCue;
class AEnemy;
class AMainPlayerController;

UENUM(BlueprintType)
enum class EMovementStatus : uint8 {
 
 EMS_Normal UMETA( DisplayName = "Normal" ),
 EMS_Sprinting UMETA(DisplayName = "Sprinting"),
 EMS_Dead UMETA(DisplayName = "Dead"),

 EMS_MAX UMETA(DisplayName = "DefaultMAX")

};

UENUM(BlueprintType)
enum class EStaminaStatus : uint8 {

  ESS_Normal UMETA(DisplayName = "Normal"),
  ESS_BelowMinimum UMETA(DisplayName = "BelowMinimum"),
  ESS_Exhausted UMETA(DisplayName = "Exhausted"),
  ESS_ExhaustedRecovering UMETA(DisplayName = "ExhaustedRecovering"),

  ESS_MAX UMETA(DisplayName = "DefaultMAX")

};

UCLASS()
class CPPULTDEVCRS_API AMain : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMain();

  TArray<FVector> PickupLocations;

  UFUNCTION(BlueprintCallable)
  void ShowPickupLocations();

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
  EStaminaStatus StaminaStatus;
  FORCEINLINE void SetStaminaStatus(EStaminaStatus Status) {
    StaminaStatus = Status;
  }

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
  float StaminaDrainRate;
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
  float MinSprintStamina;


  float InterpSpeed;
  bool bInterpToEnemy;
  void SetInterpToEnemy(bool Interp);

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
  TObjectPtr<AEnemy> CombatTarget;
  FORCEINLINE void SetCombatTarget(AEnemy* Target) {
    CombatTarget = Target;
  }
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
  bool bHasCombatTarget;
  FORCEINLINE void SetHasCombatTarget(bool HasTarget) {
    bHasCombatTarget = HasTarget;
  }

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
  FVector CombatTargetLocation;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller")
  TObjectPtr<AMainPlayerController> MainPlayerController;

  FRotator GetLookAtRotationYaw(FVector Target);

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
  EMovementStatus MovementStatus;

  /** Set Movement status and running speed */
  void SetMovementStatus(EMovementStatus Status);

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Running")
  float RunningSpeed;
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Running")
  float SprintingSpeed;

  bool bShiftKeyDown;
  
  /** Pressed down to enable sprinting */
  void ShiftKeyDown();

  /** Released to stop sprinting */
  void ShiftKeyUp();

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
  TObjectPtr<USoundCue> HitSound;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
  TObjectPtr<UParticleSystem> HitParticles;


  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anims")
  bool bAttacking;

  void Attack();

  UFUNCTION(BlueprintCallable)
  void AttackEnd();

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
  TObjectPtr<UAnimMontage> CombatMontage;

  UFUNCTION(BlueprintCallable)
  void PlaySwingSound();

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Items")
  TObjectPtr<AWeapon> EquippedWeapon;
  void SetEquippedWeapon(AWeapon* Weapon);
  FORCEINLINE AWeapon* GetEquippedWeapon() {
    return EquippedWeapon;
  }

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Items")
  TObjectPtr<AItem> ActiveOverlappingItem;
  FORCEINLINE void SetActiveOverlappingItem(AItem* Item) {
    ActiveOverlappingItem = Item;
  }

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

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStats")
  int32 Coins;



  void DecrementHealth(float Amount);

  void Die();

  void IncrementCoins(int32 Amount);

  virtual float TakeDamage(float DamageAmout, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;


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

  bool bMovingForward;
  bool bMovingRight;

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

  bool bLMBDown;
  void LMBDown();
  void LMBUp();

  UFUNCTION(BlueprintCallable)
  void DeathEnd();

  virtual void Jump() override;


};
