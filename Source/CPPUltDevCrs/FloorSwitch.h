// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloorSwitch.generated.h"

class UBoxComponent;

UCLASS()
class CPPULTDEVCRS_API AFloorSwitch : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloorSwitch();


  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Floor Switch")
  TObjectPtr<UBoxComponent> TriggerBox;
  FORCEINLINE UBoxComponent* GetTriggerBox() {
    return TriggerBox;
  }
  FORCEINLINE void SetTriggerBox(UBoxComponent* TriggerBoxComp) {
    TriggerBox = TriggerBoxComp;
  }


  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Floor Switch")
  TObjectPtr<UBoxComponent> TriggerBox2;
  FORCEINLINE UBoxComponent* GetTriggerBox2() {
    return TriggerBox2;
  }
  FORCEINLINE void SetTriggerBox2(UBoxComponent* TriggerBoxComp) {
    TriggerBox2 = TriggerBoxComp;
  }

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
  TObjectPtr<UStaticMeshComponent> FloorSwitch;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
  TObjectPtr<UStaticMeshComponent> FloorSwitch2;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
  TObjectPtr<UStaticMeshComponent> Door;

  UPROPERTY(BlueprintReadWrite, Category = "Floor Switch")
  FVector InitialDoorLocation;

  UPROPERTY(BlueprintReadWrite, Category = "Floor Switch")
  FVector InitialSwitchLocation;

  UPROPERTY(BlueprintReadWrite, Category = "Floor Switch")
  FVector InitialSwitchLocation2;

  FTimerHandle SwitchHandle;

  void CloseDoor();

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floor Switch")
  float SwitchTime;

  bool bIsCharOnSwitch;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


  UFUNCTION()
  void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
    const FHitResult& SweepResult);


  UFUNCTION()
  void OnOverlapEnd(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
    int32 OtherBodyIndex);

  UFUNCTION(BlueprintImplementableEvent, Category = "Floor Switch")
  void RaiseDoor();

  UFUNCTION(BlueprintImplementableEvent, Category = "Floor Switch")
  void LowerDoor();

  UFUNCTION(BlueprintImplementableEvent, Category = "Floor Switch")
  void RaiseFloorSwitch();

  UFUNCTION(BlueprintImplementableEvent, Category = "Floor Switch")
  void LowerFloorSwitch();

  UFUNCTION(BlueprintCallable, Category = "Floor Switch")
  void UpdateDoorLocation(float Z);

  UFUNCTION(BlueprintCallable, Category = "Floor Switch")
  void UpdateFloorSwitchLocation(float Z);



};
