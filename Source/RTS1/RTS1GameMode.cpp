// Fill out your copyright notice in the Description page of Project Settings.


#include "RTS1GameMode.h"
#include "Kismet/GameplayStatics.h"

void ARTS1GameMode::BeginPlay() {
	Super::BeginPlay();
	/*TSubclassOf<ABaseUnit> ClassToFind;
	ClassToFind = ABaseUnit::StaticClass();
	UGameplayStatics::GetAllActorsOfClass(this, ClassToFind, OwnUnits);
	int SizeOfArray = OwnUnits.Num();
	for(int i=0;i<SizeOfArray;i++)	
	{
		UE_LOG(LogTemp, Error, TEXT("Location of unit : %s"), *OwnUnits[i]->GetActorLocation().ToString());
	}*/
}
