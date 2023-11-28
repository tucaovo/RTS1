// Fill out your copyright notice in the Description page of Project Settings.


#include "RTS1HUD.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerPawn.h"


void ARTS1HUD::DrawHUD() {
	Super::DrawHUD();
	int MouseStartXPosition, MouseStartYPosition, MousePositionX, MousePositionY;
	MouseStartXPosition = Cast<APlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->MouseStartXPosition;
	MouseStartYPosition = Cast<APlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->MouseStartYPosition;
	MousePositionX = Cast<APlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->MousePositionX;
	MousePositionY = Cast<APlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->MousePositionY;
	FLinearColor Burgundy = FLinearColor(128, 0, 32, 0.2);
	DrawRect(Burgundy, MouseStartXPosition, MouseStartYPosition, MousePositionX-MouseStartXPosition, MousePositionY-MouseStartYPosition);
}
