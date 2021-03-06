// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UParticleSystemComponent;
class UParticleSystem;
class USoundCue;




UCLASS()
class CPPULTDEVCRS_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

  /* Base shape collision */
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item | Collision")
  TObjectPtr<USphereComponent> CollisionVolume;

  /* Base mesh component */
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item | Mesh")
  TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Particles")
  TObjectPtr<UParticleSystemComponent> IdleParticlesComponent;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Particles")
  TObjectPtr<UParticleSystem> OverlapParticles;
  
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sounds")
  TObjectPtr<USoundCue> OverlapSound;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Properties")
  bool bRotate;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Properties")
  float RotationRate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

  UFUNCTION()
  virtual void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
    const FHitResult& SweepResult);

  UFUNCTION()
  virtual void OnOverlapEnd(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex);

};
