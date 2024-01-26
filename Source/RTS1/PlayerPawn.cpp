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
#include "GenericPlatform/GenericPlatformMath.h"
#include "RTS1HUD.h"



APlayerPawn::APlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	UGameplayStatics::GetAllActorsOfClass(this, ABaseUnit::StaticClass(), SecondaryArray);

}

void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
	PlayerController->GetHUD()->ShowHUD();
	float UnitX, UnitY;
	for (int i = 0; i < SecondaryArray.Num(); i++) {
		if (CastChecked<ABaseUnit>(SecondaryArray[i])->IsEnemy == 0)
		{
			UnitX = SecondaryArray[i]->GetActorLocation().X;
			UnitY = SecondaryArray[i]->GetActorLocation().Y;
			SecondaryLocationArray.Add(FVector2D(UnitX, UnitY));
		}
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

	float UnitX, UnitY;
	SecondaryLocationArray.Empty();
	for (int i = 0; i < SecondaryArray.Num(); i++) {
		if (CastChecked<ABaseUnit>(SecondaryArray[i])->IsEnemy == 0)
		{
			UnitX = SecondaryArray[i]->GetActorLocation().X;
			UnitY = SecondaryArray[i]->GetActorLocation().Y;
			SecondaryLocationArray.Add(FVector2D(UnitX, UnitY));
		}
	}
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
		FirstLineTrace=BuildLineTrace(ECC_EngineTraceChannel1);
		HUD->ShowHUD();
	}
}
                     
void APlayerPawn::RightClick(const FInputActionValue& Value) {
	ARTS1PlayerController* PlayerController = CastChecked<ARTS1PlayerController>(GetController());
	PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, WorldLocation + WorldDirection * 10000.f, ECC_GameTraceChannel1);
	MouseLocationInWorld = HitResult.ImpactPoint;
	PlayerController->MoveSelectedUnits();
}
//void APlayerPawn::RightClick(const FInputActionValue& Value) {
//	ARTS1PlayerController* PlayerController = CastChecked<ARTS1PlayerController>(GetController());
//	/*PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
//	FHitResult HitResult;
//	GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, WorldLocation + WorldDirection * 10000.f, ECC_GameTraceChannel1);*/
//	MouseLocationInWorld = BuildLineTrace(ECC_EngineTraceChannel1);
//	PlayerController->MoveSelectedUnits();
//}

void APlayerPawn::Shift(const FInputActionValue& Value)
{
	bShiftPressed = Value.Get<bool>();
}

void APlayerPawn::AfterSelect(const FInputActionValue& Value)
{
	ARTS1PlayerController* PlayerController = CastChecked<ARTS1PlayerController>(GetController());
	AHUD* HUD = Cast<AHUD>(PlayerController->GetHUD());
	if (HUD->bShowHUD == true)
	{
		HUD->ShowHUD();
		SecondLineTrace = BuildLineTrace(ECC_EngineTraceChannel1);
		GetUnitsUnderRect();
	}
}

void APlayerPawn::GetUnitsUnderRect() {
	UGameplayStatics::GetAllActorsOfClass(this, ABaseUnit::StaticClass(), SecondaryArray);
	float UnitX, UnitY;
	FVector2D FirstPoint = FVector2D(FirstLineTrace.X, FirstLineTrace.Y);
	FVector2D SecondPoint = FVector2D(SecondLineTrace.X, SecondLineTrace.Y);
	ARTS1PlayerController* PlayerController = Cast<ARTS1PlayerController>(this->GetController());
	PlayerController->SelectedUnits.Empty();
	ReshapeRectangle(FirstPoint, SecondPoint);
	ABaseUnit* BaseUnit;
	for (int i = 0; i < SecondaryArray.Num(); i++) {
		UnitX = SecondaryArray[i]->GetActorLocation().X;
		UnitY = SecondaryArray[i]->GetActorLocation().Y;
		BaseUnit = Cast<ABaseUnit>(SecondaryArray[i]);
		if (UnitX >= FirstPoint.X && UnitX <= SecondPoint.X && UnitY >= FirstPoint.Y && UnitY <= SecondPoint.Y && BaseUnit->IsEnemy==0) {
			PlayerController->SelectedUnits.Add(SecondaryArray[i]);
		}
	}
}

FVector APlayerPawn::BuildLineTrace(ECollisionChannel CollisionChannel) {
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	TArray<FHitResult> OutHits;
	FHitResult OutHit;
	FVector Start;
	FVector End;
	PlayerController->DeprojectMousePositionToWorld(Start, End);
	FCollisionQueryParams CollisionParams;
		End = (End * 100000.f) + Start;
		GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, CollisionChannel, CollisionParams);
		if (OutHit.bBlockingHit) {
			return OutHit.ImpactPoint;			
		}
		else {
			return FVector(-1, -1, -1);
		}
}

void APlayerPawn::ReshapeRectangle(FVector2D& FirstPoint, FVector2D& SecondPoint) {
	FVector2D ThirdPoint, ForthPoint;
	FVector2D MockPoint = FirstPoint;
	FVector2D MockPoint2 = SecondPoint;
	ThirdPoint.X = FirstPoint.X;
	ThirdPoint.Y = SecondPoint.Y;
	ForthPoint.X = SecondPoint.X;
	ForthPoint.Y = FirstPoint.Y;
	float FirstPointValue=FirstPoint.X+FirstPoint.Y, 
		SecondPointValue=SecondPoint.X+SecondPoint.Y, 
		ThirdPointValue=ThirdPoint.X+ThirdPoint.Y, 
		ForthPointValue=ForthPoint.X+ForthPoint.Y;
	float Min = FGenericPlatformMath::Min(FGenericPlatformMath::Min(FirstPointValue, SecondPointValue), FGenericPlatformMath::Min(ThirdPointValue, ForthPointValue));
	float Max = FGenericPlatformMath::Max(FGenericPlatformMath::Max(FirstPointValue, SecondPointValue), FGenericPlatformMath::Max(ThirdPointValue, ForthPointValue));
	if (Min == FirstPointValue)
		FirstPoint = MockPoint;
	if (Min == SecondPointValue)
		FirstPoint = MockPoint2;
	if (Min == ThirdPointValue)
		FirstPoint = ThirdPoint;
	if (Min == ForthPointValue)
		FirstPoint = ForthPoint;
	if (Max == FirstPointValue)
		SecondPoint = MockPoint;
	if (Max == SecondPointValue)
		SecondPoint = MockPoint2;
	if (Max == ThirdPointValue)
		SecondPoint = ThirdPoint;
	if (Max == ForthPointValue)
		SecondPoint = ForthPoint;
}
