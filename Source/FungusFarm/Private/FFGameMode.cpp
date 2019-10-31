// Fill out your copyright notice in the Description page of Project Settings.


#include "FFGameMode.h"

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
