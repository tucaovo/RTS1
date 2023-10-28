// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseUnit.h"
#include "Components/CapsuleComponent.h"
#include "RTS1PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerPawn.h"


// Sets default values
ABaseUnit::ABaseUnit()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseUnit::BeginPlay()
{
	Super::BeginPlay();
	if (UCapsuleComponent* Capsule = GetComponentByClass<UCapsuleComponent>()) {
		Capsule->OnClicked.AddDynamic(this, &ABaseUnit::WhenClickOverTheUnit);
	}
}

// Called every frame
void ABaseUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseUnit::WhenClickOverTheUnit(UPrimitiveComponent* PrimComp, FKey InKey) {
	ARTS1PlayerController* PlayerController =Cast<ARTS1PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (this->PlayerControllerNumber==0) {
		PlayerController->SelectedUnits.Add(this);
	}
}

void ABaseUnit::MoveToLocation(FVector MouseLocationInWorld) {
	FVector InitialPos = GetRootComponent()->GetComponentLocation();
	FVector DirectionToMoveTo = MouseLocationInWorld - InitialPos;
	DirectionToMoveTo.Normalize();
	DirectionToMoveTo.Z = 0.f;
	AddMovementInput(DirectionToMoveTo);
}