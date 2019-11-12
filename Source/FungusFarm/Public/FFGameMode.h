// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SaveExtensionInterface.h"
#include "GameFramework/GameMode.h"
#include "GameplayGoal.h"
#include "GameplayGoalProvider.h"
#include "GoalsProviderComponent.h"
#include "FFGameMode.generated.h"

/**
 * 
 */
UCLASS()
class FUNGUSFARM_API AFFGameMode : public AGameMode, public ISaveExtensionInterface
{
	GENERATED_BODY()

public:
	AFFGameMode(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, Category = "Components")
		UGoalsProviderComponent* GameplayGoalsProvider = nullptr;

	// For serialization.
	UPROPERTY(BlueprintReadOnly, SaveGame, Category = "Gameplay Goals")
		TArray<FName> SecondaryProviderNames;

	// Prefix for provider properties data files.  Will look in /Game/FungusFarm/Data/. Default= "GoalProviderProperties"
	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, Category = "Gameplay Goals")
		FString ProviderPropertiesTableName;

	// Prefix for provider goals data files.  Will look in /Game/FungusFarm/Data/. Default= "SecondaryGoals"
	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, Category = "Gameplay Goals")
		FString ProviderGoalsTablePrefix;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, Category = "Save Extension")
		int32 SaveSlotId;

protected:

	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintCallable, Category = "Gameplay Goals")
		UGoalsProviderComponent* GetGoalProviderComponentByUniqueName(const FName ProviderUniqueName);

	UFUNCTION(BlueprintCallable, Category = "Gameplay Goals")
		UGoalsProviderComponent* AddSecondaryGoalProvider(const FName ProviderUniqueName);
	
	UFUNCTION(BlueprintCallable, Category = "Gameplay Goals")
		bool RemoveSecondaryGoalProvider(const FName ProviderUniqueName);

	UFUNCTION(BlueprintCallable, Category = "Gameplay Goals")
		TArray<UGoalsProviderComponent*> GetAllGoalProviders();

	UFUNCTION(BlueprintCallable, Category = "Gameplay Goals")
		TArray<UGoalsProviderComponent*> GetAllSecondaryGoalProviders();

	UFUNCTION(BlueprintCallable, Category = "Experience and Level")
		float GetExperienceRequiredForLevel(const int32 Level);

	virtual void OnSaveBegan() override;

	virtual void OnLoadBegan() override;

	virtual void OnLoadFinished(bool bError) override;
};
