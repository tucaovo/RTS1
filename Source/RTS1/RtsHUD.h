// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RtsHUD.generated.h"

/**
 * 
 */
UCLASS()
class RTS1_API ARtsHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	virtual void DrawHUD() override;
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
};
