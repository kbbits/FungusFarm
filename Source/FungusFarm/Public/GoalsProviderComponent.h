// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayGoal.h"
#include "GameplayGoalProvider.h"
#include "FungusFarm.h"
#include "GoalsProviderComponent.generated.h"


UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FUNGUSFARM_API UGoalsProviderComponent : public UActorComponent, public IGameplayGoalProvider
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGoalsProviderComponent();

protected:
	UPROPERTY(BlueprintGetter = "GetGoalProviderGuid", SaveGame, Category = "Gameplay Goals")
		FGuid GameplayGoalProviderGuid;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Goals")
		UDataTable* GoalsData;

	// Holds the goal names from the data table that are could still become available. i.e. not completed nor currently active
	TSet<FName> RemainingGoalNamesCached;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void InitGoalCache();

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay Goals")
		TArray<FGameplayGoal> GetNewGameplayGoals(const TArray<FGameplayGoal>& CurrentGoals, const TArray<FName>& CompletedGoals);
	virtual TArray<FGameplayGoal> GetNewGameplayGoals_Implementation(const TArray<FGameplayGoal>& CurrentGoals, const TArray<FName>& CompletedGoals) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay Goals")
		FGameplayGoal GetNewRandomGameplayGoal(const TArray<FGameplayGoal>& CurrentGoals, const TArray<FName>& CompletedGoals, const int MinTier = 0, const int MaxTier = 99);
	virtual FGameplayGoal GetNewRandomGameplayGoal_Implementation(const TArray<FGameplayGoal>& CurrentGoals, const TArray<FName>& CompletedGoals, const int MinTier = 0, const int MaxTier = 99) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay Goals")
		FGuid GetGameplayGoalProviderGuid();
	virtual FGuid GetGameplayGoalProviderGuid_Implementation() override;

	// For access not via interface
	UFUNCTION(BlueprintGetter, Category = "Gameplay Goals")
		FORCEINLINE FGuid GetGoalProviderGuid() { return GetGameplayGoalProviderGuid(); }
};
