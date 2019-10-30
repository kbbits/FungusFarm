// Fill out your copyright notice in the Description page of Project Settings.


#include "FFPlayerControllerBase.h"

AFFPlayerControllerBase::AFFPlayerControllerBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GameplayGoalsManager = ObjectInitializer.CreateDefaultSubobject<UActorGoalsComponent>(this, TEXT("GameplayGoalsManager"));
	//if (GameplayGoalsManager) { UE_LOG(LogTemp, Warning, TEXT("Init GoalsManager")); }
	//else { UE_LOG(LogTemp, Warning, TEXT("GoalsManager NULL")); }
}

void AFFPlayerControllerBase::OnNewGameplayGoals_Implementation(const TArray<FGameplayGoal>& NewGoals)
{
	return;
}

void AFFPlayerControllerBase::OnGameplayGoalsCompleted_Implementation(const TArray<FGameplayGoal>& NewGoals)
{
	return;
}
