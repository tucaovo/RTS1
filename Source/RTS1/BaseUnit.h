// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UObject/SparseDelegate.h"
#include "BaseUnit.generated.h"

class UStaticMeshComponent;

UCLASS()
class RTS1_API ABaseUnit : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseUnit();

	UPROPERTY(EditAnywhere, Category = "UnitDebug", BlueprintReadWrite)
		int IsEnemy = 0;
	UPROPERTY(EditAnywhere, Category = "UnitDebug")
		int Morale=100;
	UPROPERTY(EditAnywhere, Category = "UnitDebug")
		int Manpower=100;
	UPROPERTY(EditAnywhere, Category = "UnitDebug")
		int Equipment=100;
	UPROPERTY(EditAnywhere, Category = "UnitDebug")
		int Rations=100;
	UPROPERTY(EditAnywhere, Category = "UnitDebug")
		int Range = 350;

	UPROPERTY(EditAnywhere)
		TArray<AActor*> SecondaryArray;
	UPROPERTY(EditAnywhere)
		TArray<AActor*> NearUnits;

	UStaticMeshComponent* RangeSphere;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void CheckManpower();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void WhenClickOverTheUnit(UPrimitiveComponent* PrimComp, FKey InKey);

	UFUNCTION()
	void WhenRightClickOverTheUnit(UPrimitiveComponent* PrimComp, FKey InKey);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void MoveToLocation(FVector MouseLocationInWorld);

	void LookForEnemies();

	void FightBattle();

};
