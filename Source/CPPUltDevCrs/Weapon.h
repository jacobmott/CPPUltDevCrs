// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"


class USkeletalMeshComponent;
class AMain;
class USoundCue;



UENUM(BlueprintType)
enum class EWeaponState : uint8 {

  EWS_Pickup  UMETA(DisplayName = "Pickup"),
  EWS_Equipped UMETA(DisplayName = "Equipped"),

  EMS_MAX UMETA(DisplayName = "DefaultMAX")

};


/**
 * 
 */
UCLASS()
class CPPULTDEVCRS_API AWeapon : public AItem
{
	GENERATED_BODY()
	

public:

	AWeapon();

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item")
  EWeaponState WeaponState;
  FORCEINLINE void SetWeaponState(EWeaponState State) {
    WeaponState = State;
  }
  FORCEINLINE EWeaponState GetWeaponState() {
    return WeaponState;
  }

  /* Base skeletalmesh component */
  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item | SkeletalMesh")
  TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sounds")
  TObjectPtr<USoundCue> OnEquipSound;

  virtual void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
    const FHitResult& SweepResult) override;

  virtual void OnOverlapEnd(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex) override;

  void Equip(AMain* Char);

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Particles")
  bool bWeaponParticles;




};
