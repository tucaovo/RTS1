// Fill out your copyright notice in the Description page of Project Settings.


#include "RTS1PlayerController.h"
#include "PlayerPawn.h"
#include "BaseUnit.h"
#include "Kismet/GameplayStatics.h"


ARTS1PlayerController::ARTS1PlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
}

void ARTS1PlayerController::BeginPlay() {
	Super::BeginPlay();
}

void ARTS1PlayerController::MoveSelectedUnits() {
	APlayerPawn* PlayerPawn = Cast<APlayerPawn>(GetPawn());
	for (AActor* SelectedUnit : SelectedUnits) {
		Cast<ABaseUnit>(SelectedUnit)->MoveToLocation(PlayerPawn->MouseLocationInWorld); 
	}
}

void ARTS1PlayerController::AttackUnits() {
	for (AActor* SelectedUnit : SelectedUnits) {
		UE_LOG(LogTemp, Error, TEXT("Atachengo"));
	}
}
