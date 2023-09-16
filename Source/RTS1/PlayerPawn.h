// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "PlayerPawn.generated.h"


UCLASS()
class RTS1_API APlayerPawn : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerPawn();
	int MousePositionX, MousePositionY, MouseStartXPosition, MouseStartYPosition;

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
		void AfterSelect(const FInputActionValue& Value);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay() override;

};
