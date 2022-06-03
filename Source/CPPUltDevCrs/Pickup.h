// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Pickup.generated.h"


class AMain;

/**
 * 
 */
UCLASS()
class CPPULTDEVCRS_API APickup : public AItem
{
	GENERATED_BODY()
	
public:

  APickup();



  //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coins")
  //int32 CountCount;

  virtual void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
    const FHitResult& SweepResult) override;

  virtual void OnOverlapEnd(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex) override;


  UFUNCTION(BlueprintImplementableEvent, Category = "Pickup")
  void OnPickupBP(AMain* Target);

};
