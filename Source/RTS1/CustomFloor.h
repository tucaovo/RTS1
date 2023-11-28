// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "UObject/SparseDelegate.h"
#include "CustomFloor.generated.h"

/**
 * 
 */
UCLASS()
class RTS1_API ACustomFloor : public AStaticMeshActor
{
	GENERATED_BODY()
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
		void WhenClickedOverTheTerrain(UPrimitiveComponent* PrimComp, FKey InKey);
	
};
