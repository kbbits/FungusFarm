// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameplayGoal.h"
#include "GameplayGoalProvider.h"
#include "FFGameMode.generated.h"

/**
 * 
 */
UCLASS()
class FUNGUSFARM_API AFFGameMode : public AGameMode, public IGameplayGoalProvider
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintGetter="GetGoalProviderGuid", SaveGame, Category = "Gameplay Goals")
		FGuid GameplayGoalProviderGuid;

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay Goals")
		TArray<FGameplayGoal> GetNewGameplayGoals(const TArray<FGameplayGoal>& CurrentGoals, const TArray<FName>& CompletedGoals);
	virtual TArray<FGameplayGoal> GetNewGameplayGoals_Implementation(const TArray<FGameplayGoal>& CurrentGoals, const TArray<FName>& CompletedGoals) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay Goals")
		FGameplayGoal GetNewRandomGameplayGoal(const TArray<FGameplayGoal>& CurrentGoals, const TArray<FName>& CompletedGoals, const int MinTier = 0, const int MaxTier = 99);
	virtual FGameplayGoal GetNewRandomGameplayGoal_Implementation(const TArray<FGameplayGoal>& CurrentGoals, const TArray<FName>& CompletedGoals, const int MinTier = 0, const int MaxTier = 99) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay Goals")
		FGuid GetGameplayGoalProviderGuid();
	virtual FGuid GetGameplayGoalProviderGuid_Implementation() override;

	// For FFGameMode
	UFUNCTION(BlueprintGetter, Category = "Gameplay Goals")
		FORCEINLINE FGuid GetGoalProviderGuid() { return GetGameplayGoalProviderGuid(); }
};
