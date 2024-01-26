// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RTS1PlayerController.generated.h"


UCLASS()
class RTS1_API ARTS1PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ARTS1PlayerController();

	UPROPERTY(EditAnywhere, Category = "GetUnitDebug")
		TArray<AActor*> SelectedUnits;
	UPROPERTY(EditAnywhere, Category = "GetUnitDebug")
		TArray<AActor*> ToAttackUnits;

	void MoveSelectedUnits();

	void AttackUnits();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
