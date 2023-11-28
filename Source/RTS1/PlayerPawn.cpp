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
#include "Blueprint/UserWidget.h"
#include "RTS1UserWidget.h"
#include "Camera/CameraComponent.h"
#include "RTS1HUD.h"



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
	//if (RTS1UserWidgetBPClass) {
	//	RTS1UserWidget = CreateWidget<URTS1UserWidget>(PlayerController, RTS1UserWidgetBPClass);
	//}
	PlayerController->GetHUD()->ShowHUD();
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
		EnhancedInputComponent->BindAction(SelectAction, ETriggerEvent::Triggered, this, &APlayerPawn::Select);
		EnhancedInputComponent->BindAction(RightClickAction, ETriggerEvent::Triggered, this, &APlayerPawn::RightClick);
		EnhancedInputComponent->BindAction(ShiftAction, ETriggerEvent::Triggered, this, &APlayerPawn::Shift);
		EnhancedInputComponent->BindAction(AfterSelectAction, ETriggerEvent::Triggered, this, &APlayerPawn::AfterSelect);
	}
}

void APlayerPawn::Move(const FInputActionValue& Value) {
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	const FVector2d MovementVector = Value.Get<FInputActionValue::Axis2D>();
	FVector RightVector = GetActorRightVector();
	FVector ForwardVector = GetActorForwardVector();
	AddMovementInput(ForwardVector, MovementVector.X * MoveScallingFactor);
	AddMovementInput(RightVector, MovementVector.Y * MoveScallingFactor);
}

void APlayerPawn::Scroll(const FInputActionValue& Value) {
	const float IncrementValue = Value.Get<FInputActionValue::Axis1D>();
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	USpringArmComponent* SpringArmComponent = this->FindComponentByClass<USpringArmComponent>();
	SpringArmComponent->TargetArmLength = SpringArmComponent->TargetArmLength+ IncrementValue*ScrollScallingFactor;
}

void APlayerPawn::Select(const FInputActionValue& Value) {
	ARTS1PlayerController* PlayerController = CastChecked<ARTS1PlayerController>(GetController());
	double XPosition, YPosition;
	PlayerController->GetMousePosition(XPosition, YPosition);
	FVector2D FirstPoint = FVector2D(300, 300);
	FVector2D SecondPoint = FVector2D(-200, -200);
	AHUD* HUD = Cast<AHUD>(PlayerController->GetHUD());
	if (HUD->bShowHUD == false)
	{
		MouseStartXPosition = XPosition;
		MouseStartYPosition = YPosition;
		HUD->ShowHUD();
	}
}

void APlayerPawn::RightClick(const FInputActionValue& Value) {
	ARTS1PlayerController* PlayerController = CastChecked<ARTS1PlayerController>(GetController());
	PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, WorldLocation + WorldDirection * 5000.f, ECollisionChannel::ECC_WorldStatic);
	MouseLocationInWorld = HitResult.ImpactPoint;
	PlayerController->MoveSelectedUnits();
}

void APlayerPawn::Shift(const FInputActionValue& Value)
{
	bShiftPressed = Value.Get<bool>();
}

void APlayerPawn::AfterSelect(const FInputActionValue& Value)
{
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	AHUD* HUD = Cast<AHUD>(PlayerController->GetHUD());
	if (HUD->bShowHUD == true)
	{
		HUD->ShowHUD();
		GetUnitsUnderRect();
	}
}

void APlayerPawn::GetUnitsUnderRect() {
	UGameplayStatics::GetAllActorsOfClass(this, ABaseUnit::StaticClass(), SecondaryArray);
}

FVector APlayerPawn::BuildLineTrace() {
	FHitResult OutHit;
	FVector Start;
	FVector End;
	FCollisionQueryParams CollisionParams;
	if (GetComponentByClass<UCameraComponent>()) {
		Start = GetComponentByClass<UCameraComponent>()->GetComponentLocation();
		End = (GetComponentByClass<UCameraComponent>()->GetForwardVector() * 1000.f) + Start;
		GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams);
		if (OutHit.bBlockingHit) {
			return OutHit.ImpactPoint;
		}
		else {
			return FVector(0, 0, 0);
		}
	}
	else {
		return FVector(0, 0, 0);
	}
}
