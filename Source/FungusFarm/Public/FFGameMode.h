// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameplayGoal.h"
#include "GameplayGoalProvider.h"
#include "GoalsProviderComponent.h"
#include "FFGameMode.generated.h"

/**
 * 
 */
UCLASS()
class FUNGUSFARM_API AFFGameMode : public AGameMode 
{
	GENERATED_BODY()

public:
	AFFGameMode(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, Category = "Components")
		UGoalsProviderComponent* GameplayGoalsProvider = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, Category = "Components")
		TArray<UGoalsProviderComponent*> SecondaryGoalProviders;

	// Prefix for provider properties data files.  Will look in /Game/FungusFarm/Data/. Default= "GoalProviderProperties"
	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, Category = "Components")
		FString ProviderPropertiesTableName;

	// Prefix for provider goals data files.  Will look in /Game/FungusFarm/Data/. Default= "SecondaryGoals"
	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, Category = "Components")
		FString ProviderGoalsTablePrefix;

protected:

	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintCallable, Category = "Gameplay Goals")
		UGoalsProviderComponent* GetGoalProviderComponentByUniqueName(const FName ProviderUniqueName);

	UFUNCTION(BlueprintCallable, Category = "Gameplay Goals")
		UGoalsProviderComponent* AddSecondaryGoalProvider(const FName ProviderUniqueName);
	
	UFUNCTION(BlueprintCallable, Category = "Gameplay Goals")
		bool RemoveSecondaryGoalProvider(const FName ProviderUniqueName);
};
