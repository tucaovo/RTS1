// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/HUD.h"
#include "RTS1PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "BaseUnit.h"
#include "EnhancedInputComponent.h"


APlayerPawn::APlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
}

void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	double MouseX, MouseY;
	PlayerController->GetMousePosition(MouseX, MouseY);
	MousePositionX = MouseX;
	MousePositionY = MouseY;
}

void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(ScrollAction, ETriggerEvent::Triggered, this, &APlayerPawn::Scroll);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerPawn::Move);
		EnhancedInputComponent->BindAction(SelectAction, ETriggerEvent::Ongoing, this, &APlayerPawn::Select);
		EnhancedInputComponent->BindAction(AfterSelectAction, ETriggerEvent::Triggered, this, &APlayerPawn::AfterSelect);
		EnhancedInputComponent->BindAction(ClickSelectAction, ETriggerEvent::Triggered, this, &APlayerPawn::ClickSelect);
		EnhancedInputComponent->BindAction(RightClickAction, ETriggerEvent::Triggered, this, &APlayerPawn::RightClick);
	}
}

void APlayerPawn::Move(const FInputActionValue& Value) {
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	const FVector2d MovementVector = Value.Get<FInputActionValue::Axis2D>();
	FVector RightVector = GetActorRightVector();
	FVector ForwardVector = GetActorForwardVector();
	AddMovementInput(ForwardVector, MovementVector.X*MoveScallingFactor);
	AddMovementInput(RightVector, MovementVector.Y * MoveScallingFactor);
}

void APlayerPawn::Scroll(const FInputActionValue& Value) {
	const float IncrementValue = Value.Get<FInputActionValue::Axis1D>();
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	USpringArmComponent* SpringArmComponent = this->FindComponentByClass<USpringArmComponent>();
	SpringArmComponent->TargetArmLength = SpringArmComponent->TargetArmLength+ IncrementValue*ScrollScallingFactor;
}

void APlayerPawn::Select(const FInputActionValue& Value) {
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	AHUD* Hud = PlayerController->GetHUD();
	Hud->bShowHUD = true;
	double XPosition, YPosition;
	PlayerController->GetMousePosition(XPosition,YPosition);
	MouseStartXPosition=XPosition;
	MouseStartYPosition = YPosition;
}

void APlayerPawn::AfterSelect(const FInputActionValue& Value) {
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	AHUD* Hud = PlayerController->GetHUD();
	Hud->bShowHUD = false;
}

void APlayerPawn::ClickSelect(const FInputActionValue& Value) {
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	double XPosition, YPosition;
	PlayerController->GetMousePosition(XPosition, YPosition);
	PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
	//UE_LOG(LogTemp, Error, TEXT("World Location = %s  ,  World Direction = %s"), *WorldLocation.ToString(), *WorldDirection.ToString());
}

void APlayerPawn::RightClick(const FInputActionValue& Value) {
	ARTS1PlayerController* PlayerController = CastChecked<ARTS1PlayerController>(GetController());
	double XPosition, YPosition;
	PlayerController->GetMousePosition(XPosition, YPosition);
	PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
	UE_LOG(LogTemp, Error, TEXT("World Location = %s  ,  World Direction = %s"), *WorldLocation.ToString(), *WorldDirection.ToString());
	PlayerController->MoveSelectedUnits();
}
