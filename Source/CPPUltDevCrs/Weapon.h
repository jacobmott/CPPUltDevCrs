// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"


class USkeletalMeshComponent;
class AMain;


/**
 * 
 */
UCLASS()
class CPPULTDEVCRS_API AWeapon : public AItem
{
	GENERATED_BODY()
	

public:

	AWeapon();


  /* Base skeletalmesh component */
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item | SkeletalMesh")
  TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;


  virtual void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
    const FHitResult& SweepResult) override;

  virtual void OnOverlapEnd(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex) override;

  void Equip(AMain* Char);




};
