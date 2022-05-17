// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingPlatform.generated.h"


class UStaticMeshComponent;

UCLASS()
class CPPULTDEVCRS_API AFloatingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloatingPlatform();

  /* Mesh for the platform */
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Platform")
  TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, Category = "Platform")
  FVector StartPoint;


  UPROPERTY(EditAnywhere, Category = "Platform", meta = (MakeEditWidget = "true"))
  FVector EndPoint;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform")
  float InterpSpeed;
	
	FTimerHandle InterpTimer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform")
  bool AreInterping;


  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform")
  float InterpTime;


  float TotalDistance;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

  void ToggleInterping();

  void SwapVectors(FVector& VecOne, FVector& VecTwo);

};
