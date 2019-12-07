// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Engine/Texture2D.h"
#include "Guid.h"
#include "GameplayGoal.h"
#include "GameplayGoalProvider.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGameplayGoalProvider : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FUNGUSFARM_API IGameplayGoalProvider
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.

public:
	//bool operator== (const TScriptInterface<IGameplayGoalProvider>& other) const;
	//bool operator== (const TScriptInterface<IGameplayGoalProvider>& other);
	//bool operator== (TScriptInterface<IGameplayGoalProvider>& other);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay Goals")
		TArray<FGameplayGoal> GetNewGameplayGoals(const TArray<FGameplayGoal>& CurrentGoals, const TArray<FName>& CompletedGoals, const TArray<FName>& AbandonedGoals, const float CurrentExperienceLevel);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay Goals")
		void OnGameplayGoalCompleted(const FGameplayGoal& CompletedGoal);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay Goals")
		void OnGameplayGoalAbandoned(const FGameplayGoal& CompletedGoal);

	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay Goals")
	//	FGameplayGoal GetNewRandomGameplayGoal(const TArray<FGameplayGoal>& CurrentGoals, const TArray<FName>& CompletedGoals, const int MinTier = 0, const int MaxTier = 99);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay Goals")
		FGuid GetGameplayGoalProviderGuid();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay Goals")
		FName GetGameplayGoalProviderUniqueName();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay Goals")
		FString GetGameplayGoalProviderFriendlyName();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay Goals")
		TAssetPtr<UTexture2D> GetGameplayGoalProviderImage();
};
