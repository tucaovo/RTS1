// Fill out your copyright notice in the Description page of Project Settings.


#include "RtsHUD.h"
#include "PlayerPawn.h"

void ARtsHUD::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void ARtsHUD::BeginPlay() {
	Super::BeginPlay();
	bShowHUD = false;
}

void ARtsHUD::DrawHUD() {
	Super::DrawHUD();
	static const FLinearColor LineColor = FLinearColor(128.f, 128.f, 128.f, 0.3f);
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	APlayerPawn* PlayerPawn = CastChecked<APlayerPawn>(PlayerController->GetPawn());
	if (PlayerPawn->MouseStartXPosition && PlayerPawn->MouseStartYPosition)
	{
		int Width = PlayerPawn->MouseStartXPosition - PlayerPawn->MousePositionX;
		int Height = PlayerPawn->MouseStartYPosition - PlayerPawn->MousePositionY;
		DrawRect(LineColor, PlayerPawn->MouseStartXPosition, PlayerPawn->MouseStartYPosition,-Width,-Height);
	}
}