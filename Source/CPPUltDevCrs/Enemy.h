// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"


class USphereComponent;
class AAIController;
class AMain;

UENUM(BlueprintType)
enum class EEnemyMovementStatus : uint8 {

  EMS_Idle UMETA(DisplayName = "Idle"),
  EMS_MoveToTarget UMETA(DisplayName = "MoveToTarget"),
  EMS_Attacking UMETA(DisplayName = "Attacking"),

  EMS_MAX UMETA(DisplayName = "DefaultMAX")

};

UCLASS()
class CPPULTDEVCRS_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	EEnemyMovementStatus EnemyMovementStatus;

	FORCEINLINE void SetEnemyMovementStatus(EEnemyMovementStatus Status) {
    EnemyMovementStatus = Status;
	}


  /* Base shape collision */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
  TObjectPtr<USphereComponent> AgroSphere;


  /* Base shape collision */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
  TObjectPtr<USphereComponent> CombatSphere;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

  UFUNCTION()
  void AgroSphereOnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
    const FHitResult& SweepResult);

  UFUNCTION()
  void AgroSphereOnOverlapEnd(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex);

  UFUNCTION()
  void CombatSphereOnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
    const FHitResult& SweepResult);

  UFUNCTION()
  void CombatSphereOnOverlapEnd(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex);

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
  TObjectPtr<AAIController> AIController;

  UFUNCTION(BlueprintCallable)
  void MoveToTarget(AMain* Target);

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
  bool bOverlappingCombatSphere;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
  TObjectPtr<AMain> CombatTarget;
  


};
