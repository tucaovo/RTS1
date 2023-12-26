// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseUnit.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerPawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "RTS1PlayerController.h"

ABaseUnit::ABaseUnit()
{
	PrimaryActorTick.bCanEverTick = true;
	UGameplayStatics::GetAllActorsOfClass(this, ABaseUnit::StaticClass(), SecondaryArray);
}

void ABaseUnit::BeginPlay()
{
	Super::BeginPlay();
	if (UCapsuleComponent* Capsule = GetComponentByClass<UCapsuleComponent>()) {
		Capsule->OnClicked.AddDynamic(this, &ABaseUnit::WhenClickOverTheUnit);
	}
}

void ABaseUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	LookForEnemies();
}

void ABaseUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseUnit::WhenClickOverTheUnit(UPrimitiveComponent* PrimComp, FKey InKey) {
	ARTS1PlayerController* PlayerController =Cast<ARTS1PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	APawn* PlayerPawn = PlayerController->GetPawn();
	if (this->IsEnemy==0 && Cast<APlayerPawn>(PlayerPawn)->bShiftPressed==1) {
		PlayerController->SelectedUnits.Add(this);
	}
	else if (this->IsEnemy == 0 && Cast<APlayerPawn>(PlayerPawn)->bShiftPressed == 0) {
		PlayerController->SelectedUnits.Empty();
		PlayerController->SelectedUnits.Add(this);
	}
	else {
		PlayerController->SelectedUnits.Empty();
	}
}

void ABaseUnit::MoveToLocation(FVector MouseLocationInWorld) {
	float FinishX= MouseLocationInWorld.X, FinishY=MouseLocationInWorld.Y;
	FVector InitialPos = GetRootComponent()->GetComponentLocation();
	FVector DirectionToMoveTo = MouseLocationInWorld - InitialPos;
	DirectionToMoveTo.Normalize();
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), MouseLocationInWorld);

}

void ABaseUnit::LookForEnemies() {
	FVector UnitPosition = this->GetActorLocation();
	float X = UnitPosition.X;
	float Y = UnitPosition.Y;
	FVector ForwardVector(X + Range, Y, 0);
	FVector RightVector(X , Y + Range, 0);
	FVector BackwardVector(X - Range, Y, 0);
	FVector LeftVector(X, Y - Range, 0);
	for (int i = 0; i <= Range; i++) {

	}
}