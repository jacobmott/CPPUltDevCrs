// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Explosive.generated.h"

/**
 * 
 */
UCLASS()
class CPPULTDEVCRS_API AExplosive : public AItem
{
	GENERATED_BODY()
  
public:

  AExplosive();

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
  TSubclassOf<UDamageType> DamageTypeClass;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
  float Damage;

  virtual void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
    const FHitResult& SweepResult) override;

  virtual void OnOverlapEnd(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex) override;

	
};
