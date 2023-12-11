// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "PlayerPawn.generated.h"

class URTS1UserWidget;
class ABaseUnit;

UCLASS()
class RTS1_API APlayerPawn : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerPawn();
	int MousePositionX, MousePositionY, MouseStartXPosition, MouseStartYPosition;

	UPROPERTY(EditAnywhere);
	bool bShiftPressed = false;

	FVector MouseLocationInWorld;
	FVector WorldLocation, WorldDirection;
	FVector FirstLineTrace, SecondLineTrace;

	UPROPERTY(EditAnywhere)
		TSubclassOf<ABaseUnit> BaseUnitClass;
	UPROPERTY(EditAnywhere)
		TSubclassOf<URTS1UserWidget> RTS1UserWidgetBPClass;
	UPROPERTY()
		URTS1UserWidget* RTS1UserWidget;
	UPROPERTY(EditAnywhere)
		TArray<AActor*> SecondaryArray;

protected:
	// Called when the game starts or when spawned
	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
		UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
		UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
		UInputAction* ScrollAction;
	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
		UInputAction* SelectAction;
	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
		UInputAction* RightClickAction;
	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
		UInputAction* ShiftAction;
	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
		UInputAction* AfterSelectAction;

	UPROPERTY(EditAnywhere, Category = "Scalling")
		float ScrollScallingFactor;
	UPROPERTY(EditAnywhere, Category = "Scalling")
		float MoveScallingFactor;

	UFUNCTION()
		void Select(const FInputActionValue& Value);
	UFUNCTION()
		void Move(const FInputActionValue& Value);
	UFUNCTION()
		void Scroll(const FInputActionValue& Value);
	UFUNCTION()
		void RightClick(const FInputActionValue& Value);
	UFUNCTION()
		void Shift(const FInputActionValue& Value);
	UFUNCTION()
		void AfterSelect(const FInputActionValue& Value);



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void GetUnitsUnderRect();

	virtual void BeginPlay() override;

	FVector BuildLineTrace();

	void ReshapeRectangle(FVector2D& FirstPoint, FVector2D& SecondPoint);

};
