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
	LookForEnemies(); 
}

void ABaseUnit::BeginPlay()
{
	Super::BeginPlay();
	if (UCapsuleComponent* Capsule = GetComponentByClass<UCapsuleComponent>()) {
		Capsule->OnClicked.AddDynamic(this, &ABaseUnit::WhenClickOverTheUnit);
	}
	RangeSphere = GetComponentByClass<UStaticMeshComponent>();
	if (RangeSphere)
	{
		RangeSphere->SetWorldScale3D(FVector((Range / 100) * 4, (Range / 100) * 4, (Range / 100) * 4));
		RangeSphere->OnComponentBeginOverlap.AddDynamic(this, &ABaseUnit::OnOverlapBegin);
		RangeSphere->OnComponentEndOverlap.AddDynamic(this, &ABaseUnit::OnOverlapEnd);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Missing Range"));//without this if else it crashes PROBLEMS
	}
}

void ABaseUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RangeSphere->GetOverlappingActors(NearUnits, ABaseUnit::StaticClass());
	LookForEnemies();
	CheckManpower();
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

void ABaseUnit::WhenRightClickOverTheUnit(UPrimitiveComponent* PrimComp, FKey InKey) {
	ARTS1PlayerController* PlayerController = Cast<ARTS1PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	APawn* PlayerPawn = PlayerController->GetPawn();
	//if (this->IsEnemy == 1 && PlayerController->bRightClickPressed == 1 /*&& Cast<APlayerPawn>(PlayerPawn)->bShiftPressed == 1*/) {
	//}
}

void ABaseUnit::MoveToLocation(FVector MouseLocationInWorld) {
	float FinishX= MouseLocationInWorld.X, FinishY=MouseLocationInWorld.Y;
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), MouseLocationInWorld);
}

void ABaseUnit::LookForEnemies() {
	if (RangeSphere) {
		RangeSphere->GetOverlappingActors(NearUnits, ABaseUnit::StaticClass());
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("MissingRangeSphere"));//PROBLEMS
	}
	for (int i = 0; i < NearUnits.Num(); i++) {
		if (this != NearUnits[i])
		{
			if (CastChecked<ABaseUnit>(NearUnits[i])->IsEnemy != this->IsEnemy)
				FightBattle();
			else if (CastChecked<ABaseUnit>(NearUnits[i])->IsEnemy == this->IsEnemy)
				Morale = Morale + 0.1;
		}
	}
}

void ABaseUnit::FightBattle() {
	int Chance = FMath::RandRange(0, 60);
	if (Chance == 1 )
		Manpower = Manpower - 1;
	if (Chance == 2 || Chance == 3 || Chance == 4)
		Morale = Morale - 1;
	if (Chance == 5 || Chance == 6 || Chance == 7 || Chance == 8)
		Equipment = Equipment - 1;
}

void ABaseUnit::CheckManpower()
{
	if (this->Manpower <= 0)
		this->Destroy();
}

void ABaseUnit::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (CastChecked<ABaseUnit>(OtherActor)->IsEnemy == 1)
	{
		OtherActor->SetActorHiddenInGame(false);
	}
}

void ABaseUnit::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (CastChecked<ABaseUnit>(OtherActor)->IsEnemy == 1)
	{
		OtherActor->SetActorHiddenInGame(true);
	}

}