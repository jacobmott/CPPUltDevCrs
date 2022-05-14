// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Collider.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class USpringArmComponent;
class UCameraComponent;
class UColliderMovementComponent;

UCLASS()
class CPPULTDEVCRS_API ACollider : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACollider();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

  UPROPERTY(VisibleAnywhere, Category = "Mesh")
  TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
  FORCEINLINE UStaticMeshComponent* GetMeshComponent() {
    return StaticMeshComponent;
  }
  FORCEINLINE void SetMeshComponent(UStaticMeshComponent* MeshComponent) {
    StaticMeshComponent = MeshComponent;
  }

  UPROPERTY(VisibleAnywhere, Category = "Mesh")
  TObjectPtr<USphereComponent> SphereComponent;
  FORCEINLINE USphereComponent* GetSphereComponent() {
    return SphereComponent;
  }
  FORCEINLINE void SetSphereComponent(USphereComponent* SphereComp) {
    SphereComponent = SphereComp;
  }

  UPROPERTY(VisibleAnywhere, Category = "Mesh")
  TObjectPtr<UCameraComponent> CameraComponent;
  FORCEINLINE UCameraComponent* GetCameraComponent() {
    return CameraComponent;
  }
  FORCEINLINE void SetCameraComponent(UCameraComponent* CameraComp) {
    CameraComponent = CameraComp;
  }

  UPROPERTY(VisibleAnywhere, Category = "Mesh")
  TObjectPtr<USpringArmComponent> SpringArmComponent;
  FORCEINLINE USpringArmComponent* GetSpringArmComponent() {
    return SpringArmComponent;
  }
  FORCEINLINE void SetSpringArmComponent(USpringArmComponent* SpringArmComp) {
    SpringArmComponent = SpringArmComp;
  }


  UPROPERTY(VisibleAnywhere, Category = "Mesh")
  TObjectPtr<UColliderMovementComponent> OurMovementComponent;

  virtual UPawnMovementComponent* GetMovementComponent() const override;

  //UPROPERTY(EditAnywhere, Category = "PawnMovement")
  //float MaxSpeed;



private:

  void MoveForward(float Value);
  void MoveRight(float Value);

  //FVector CurrentVelocity;


};
