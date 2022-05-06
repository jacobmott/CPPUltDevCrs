// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Floater.generated.h"

class UStaticMeshComponent;

UCLASS()
class CPPULTDEVCRS_API AFloater : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloater();

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ActorMeshComponents")
  TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

  //Location used by set actor location when begin play is called
  UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "FloaterVariables")
  FVector InitialLocation;

  UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "FloaterVariables")
  //Location of the actor when dragged in from the editor
  FVector PlacedLocation = FVector(0.0f, 0.0f, 0.0f);

  //
  UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "FloaterVariables")
  FVector WorldOrigin;

  //
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FloaterVariables")
  FVector InitialDirection;

  //
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FloaterVariables")
  bool bShouldFloat;


  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FloaterVariables")
  bool bInitializedFloaterLocations;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
