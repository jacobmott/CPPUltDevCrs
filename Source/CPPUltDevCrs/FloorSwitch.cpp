// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorSwitch.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"

// Sets default values
AFloorSwitch::AFloorSwitch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

  TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
  RootComponent = TriggerBox;

  //We only want to query when we get overlap, we don't care about physics and the physics responses/system
  TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
  TriggerBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
  TriggerBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
  TriggerBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

  TriggerBox->SetBoxExtent(FVector(62.0f, 62.0f, 32.0f));
  

  FloorSwitch = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorSwitch"));
  FloorSwitch->SetupAttachment(GetRootComponent());

  Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
  Door->SetupAttachment(GetRootComponent());

  SwitchTime = 2.0f;

  bIsCharOnSwitch = false;



  TriggerBox2 = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox2"));

  //We only want to query when we get overlap, we dont care about phyics and the phyiscs responses/system
  TriggerBox2->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
  TriggerBox2->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
  TriggerBox2->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
  TriggerBox2->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

  TriggerBox2->SetBoxExtent(FVector(62.0f, 62.0f, 32.0f));
  TriggerBox2->SetupAttachment(GetRootComponent());

  FloorSwitch2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorSwitch2"));
  FloorSwitch2->SetupAttachment(GetRootComponent());







}

void AFloorSwitch::CloseDoor()
{

  if (!bIsCharOnSwitch){
    LowerDoor();
    RaiseFloorSwitch();
  }
}

void AFloorSwitch::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
  //UE_LOG(LogTemp, Warning, TEXT("Overlap Begin"));
  //GetWorldTimerManager().ClearTimer(SwitchHandle);
  if (!bIsCharOnSwitch) { bIsCharOnSwitch = true; }
  RaiseDoor();
  LowerFloorSwitch();
}



void AFloorSwitch::OnOverlapEnd(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

  //UE_LOG(LogTemp, Warning, TEXT("Overlap End"));
  //LowerDoor();
  //RaiseFloorSwitch();
  if (bIsCharOnSwitch) { bIsCharOnSwitch = false; }
  GetWorldTimerManager().SetTimer(SwitchHandle, this, &AFloorSwitch::CloseDoor, SwitchTime);
}


void AFloorSwitch::UpdateDoorLocation(float Z)
{
  FVector NewLocation = InitialDoorLocation;
  NewLocation.Z += Z;
  Door->SetWorldLocation(NewLocation);

  //int intVar = 5;

  //float floatVar = 3.7f;

  //FString fstringVar = "an fstring variable";

  //**UE_LOG(LogTemp, Warning, TEXT("Text, %d %f %s"), intVar, floatVar, *fstringVar);
  //UE_LOG(LogTemp, Warning, TEXT("Z, %f"), Z);
  //UE_LOG(LogTemp, Warning, TEXT("InitialDoorLocation, %f, %f, %f"), InitialDoorLocation.X, InitialDoorLocation.Y, InitialDoorLocation.Z);
  //UE_LOG(LogTemp, Warning, TEXT("NewLocation, %f, %f, %f"), NewLocation.X, NewLocation.Y, NewLocation.Z);


}

void AFloorSwitch::UpdateFloorSwitchLocation(float Z)
{
  FVector NewLocation = InitialSwitchLocation;
  NewLocation.Z += Z;
  FloorSwitch->SetWorldLocation(NewLocation);

  FVector NewLocation2 = InitialSwitchLocation2;
  NewLocation2.Z += Z;
  FloorSwitch2->SetWorldLocation(NewLocation2);
}

// Called when the game starts or when spawned
void AFloorSwitch::BeginPlay()
{
	Super::BeginPlay();
  TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AFloorSwitch::OnOverlapBegin);
  TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AFloorSwitch::OnOverlapEnd);

  InitialSwitchLocation = FloorSwitch->GetComponentLocation();
  InitialDoorLocation = Door->GetComponentLocation();


  TriggerBox2->OnComponentBeginOverlap.AddDynamic(this, &AFloorSwitch::OnOverlapBegin);
  TriggerBox2->OnComponentEndOverlap.AddDynamic(this, &AFloorSwitch::OnOverlapEnd);
  InitialSwitchLocation2 = FloorSwitch2->GetComponentLocation();
}

// Called every frame
void AFloorSwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

