// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelTransitionVolume.generated.h"

class UBoxComponent;
class UBillboardComponent;

UCLASS()
class CPPULTDEVCRS_API ALevelTransitionVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelTransitionVolume();

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Transition")
  TObjectPtr<UBoxComponent> TransitionVolume;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Billboard")
  TObjectPtr<UBillboardComponent> BillboardComp;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Transition")
  FName TransitionLevelName;

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
