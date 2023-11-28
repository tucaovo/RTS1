// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RTS1HUD.generated.h"

/**
 * 
 */
UCLASS()
class RTS1_API ARTS1HUD : public AHUD
{
	GENERATED_BODY()
		virtual void DrawHUD() override;
};
