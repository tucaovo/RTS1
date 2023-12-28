// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseUnit.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerPawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/SphereComponent.h"
#include "RTS1PlayerController.h"

ABaseUnit::ABaseUnit()
{
	PrimaryActorTick.bCanEverTick = true;
	UGameplayStatics::GetAllActorsOfClass(this, ABaseUnit::StaticClass(), SecondaryArray);
}

void ABaseUnit::BeginPlay()
{
	Super::BeginPlay();
	if (UCapsuleComponent* Capsule = GetComponentByClass<UCapsuleComponent>()) {
		Capsule->OnClicked.AddDynamic(this, &ABaseUnit::WhenClickOverTheUnit);
	}
	RangeSphere = GetComponentByClass<UStaticMeshComponent>();
	if (RangeSphere) {
		UE_LOG(LogTemp, Warning, TEXT("Found it!"));
		RangeSphere->SetWorldScale3D(FVector((Range/100)*2.4, (Range / 100) * 2.4, (Range / 100) * 2.4));
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("nu!"));
	}
	
}

void ABaseUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RangeSphere->GetOverlappingActors(NearUnits, ABaseUnit::StaticClass());
	LookForEnemies();
}

void ABaseUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseUnit::WhenClickOverTheUnit(UPrimitiveComponent* PrimComp, FKey InKey) {
	ARTS1PlayerController* PlayerController =Cast<ARTS1PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	APawn* PlayerPawn = PlayerController->GetPawn();
	if (this->IsEnemy==0 && Cast<APlayerPawn>(PlayerPawn)->bShiftPressed==1) {
		PlayerController->SelectedUnits.Add(this);
	}
	else if (this->IsEnemy == 0 && Cast<APlayerPawn>(PlayerPawn)->bShiftPressed == 0) {
		PlayerController->SelectedUnits.Empty();
		PlayerController->SelectedUnits.Add(this);
	}
	else {
		PlayerController->SelectedUnits.Empty();
	}
}

void ABaseUnit::MoveToLocation(FVector MouseLocationInWorld) {
	float FinishX= MouseLocationInWorld.X, FinishY=MouseLocationInWorld.Y;
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), MouseLocationInWorld);
}

void ABaseUnit::LookForEnemies() {
	FVector UnitPosition = this->GetActorLocation();
	float X = UnitPosition.X;
	float Y = UnitPosition.Y;
	FVector ForwardVector(X + Range, Y, 0);
	FVector RightVector(X , Y + Range, 0);
	FVector BackwardVector(X - Range, Y, 0);
	FVector LeftVector(X, Y - Range, 0);
	DrawDebugLine(GetWorld(), ForwardVector, ForwardVector * (0, 0, 100), FColor::Black, false, 0.1f);
	DrawDebugLine(GetWorld(), RightVector, RightVector * (0, 0, 100), FColor::Black, false, 0.1f);
	DrawDebugLine(GetWorld(), BackwardVector, BackwardVector * (0, 0, 100), FColor::Black, false, 0.1f);
	DrawDebugLine(GetWorld(), LeftVector, LeftVector * (0, 0, 100), FColor::Black, false, 0.1f);
}