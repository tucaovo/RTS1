// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RTS1UserWidget.generated.h"

/**
 * 
 */
class UImage;
class UBorder;

UCLASS()
class RTS1_API URTS1UserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void ResizeBox(int MouseStartXPosition, int MouseStartYPosition, int MousePositionX, int MousePositionY);
	UPROPERTY(EditAnywhere, meta=(BindWidget))
		UImage* Box;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		UBorder* Border;
		
};
