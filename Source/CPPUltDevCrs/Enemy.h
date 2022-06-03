// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"


class USphereComponent;
class AAIController;
class AMain;
class UParticleSystem;
class USoundCue;
class UBoxComponent;
class UAnimMontage;



UENUM(BlueprintType)
enum class EEnemyMovementStatus : uint8 {

  EMS_Idle UMETA(DisplayName = "Idle"),
  EMS_MoveToTarget UMETA(DisplayName = "MoveToTarget"),
  EMS_Attacking UMETA(DisplayName = "Attacking"),
  EMS_Dead UMETA(DisplayName = "Dead"),

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
  FORCEINLINE EEnemyMovementStatus GetEnemyMovementStatus() {
    return EnemyMovementStatus;
  }


  /* Base shape collision */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
  TObjectPtr<USphereComponent> AgroSphere;


  /* Base shape collision */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
  TObjectPtr<USphereComponent> CombatSphere;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
  float Health;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
  float MaxHealth;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
  float Damage;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
  TObjectPtr<UParticleSystem> HitParticles;

  virtual float TakeDamage(float DamageAmout, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
  void Die(AActor* Causer);

  UFUNCTION(BlueprintCallable)
  void DeathEnd();

  bool Alive();

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
  TObjectPtr<USoundCue> HitSound;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
  TObjectPtr<USoundCue> SwingSound;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
  TObjectPtr<UBoxComponent> CombatCollision;

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

  UFUNCTION(BlueprintCallable)
  void AttackEnd();

  void Attack();

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
  bool bAttacking;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
  TObjectPtr<UAnimMontage> CombatMontage;

  FTimerHandle AttackTimer;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
  float AttackMinTime;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
  float AttackMaxTime;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
  TSubclassOf<UDamageType> DamageTypeClass; 


  FTimerHandle DeathTimer;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
  float DeathDelay;

  bool bHasValidTarget;


  void Dissappear();

  UFUNCTION()
  void CombatOnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
    const FHitResult& SweepResult);

  UFUNCTION()
  void CombatOnOverlapEnd(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex);
  


  UFUNCTION(BlueprintCallable)
  void ActivateCollision();

  UFUNCTION(BlueprintCallable)
  void DeactivateCollision();


};
