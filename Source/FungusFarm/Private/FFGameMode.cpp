// Fill out your copyright notice in the Description page of Project Settings.


#include "FFGameMode.h"

AFFGameMode::AFFGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GameplayGoalsProvider = ObjectInitializer.CreateDefaultSubobject<UGoalsProviderComponent>(this, TEXT("GoalsProvider"));
	UE_LOG(LogFFGame, VeryVerbose, TEXT("FFGameMode constructed. Goals Provider %s"), (GameplayGoalsProvider == nullptr ? TEXT("INVALID") : TEXT("VALID")));
}

/*
TArray<FGameplayGoal> AFFGameMode::GetNewGameplayGoals_Implementation(const TArray<FGameplayGoal>& CurrentGoals, const TArray<FName>& CompletedGoals)
{
	// Subclass should implement meaningful logic
	TArray<FGameplayGoal> NewGoals = TArray<FGameplayGoal>();
	   
	return NewGoals;
}

FGameplayGoal AFFGameMode::GetNewRandomGameplayGoal_Implementation(const TArray<FGameplayGoal>& CurrentGoals, const TArray<FName>& CompletedGoals, const int MinTier, const int MaxTier)
{
	// Subclass should implement meaningful logic
	return FGameplayGoal();
}

FGuid AFFGameMode::GetGameplayGoalProviderGuid_Implementation()
{
	// Lazy init
	if (!GameplayGoalProviderGuid.IsValid())
	{
		GameplayGoalProviderGuid = FGuid::NewGuid();
	}
	return GameplayGoalProviderGuid;
}
*/