// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayGoal.h"
#include "GameplayGoalListener.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGameplayGoalListener : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FUNGUSFARM_API IGameplayGoalListener
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay Goals")
		void OnNewGameplayGoals(const TArray<FGameplayGoal>& NewGoals);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay Goals")
		void OnGameplayGoalsCompleted(const TArray<FGameplayGoal>& CompletedGoals);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay Goals")
		void OnGameplayGoalAbandoned(const FName& AbandonedGoalName);
};
