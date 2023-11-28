// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomFloor.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerPawn.h"
#include "RTS1PlayerController.h"

void ACustomFloor::BeginPlay()
{
	Super::BeginPlay();
	if (UStaticMeshComponent* StaticMesh = GetComponentByClass<UStaticMeshComponent>())
		StaticMesh->OnClicked.AddDynamic(this, &ACustomFloor::WhenClickedOverTheTerrain);
}

void ACustomFloor::WhenClickedOverTheTerrain(UPrimitiveComponent* PrimComp, FKey InKey) {
	ARTS1PlayerController* PlayerController = Cast<ARTS1PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	PlayerController->SelectedUnits.Empty();
}