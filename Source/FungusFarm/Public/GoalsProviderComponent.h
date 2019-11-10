// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Delegates/Delegate.h"
#include "GameplayGoal.h"
#include "GameplayGoalProvider.h"
#include "FungusFarm.h"
#include "GoalsProviderComponent.generated.h"

// Event dispatcher for when we enable new goals
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNewGoalsEnabled);

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FUNGUSFARM_API UGoalsProviderComponent : public UActorComponent, public IGameplayGoalProvider
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGoalsProviderComponent();

	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
		FOnNewGoalsEnabled OnNewGoalsEnabled;

	UPROPERTY(EditAnywhere, SaveGame, Category = "Gameplay Goals")
		FName UniqueName;

	UPROPERTY(EditAnywhere, SaveGame, Category = "Gameplay Goals")
		FString FriendlyName;

	UPROPERTY(EditAnywhere, Category = "Gameplay Goals")
		UDataTable* GoalsData;

	// Setting this to true will cause GetNewGameplayGoals to always return an empty list. i.e no new goals.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, Category = "Gameplay Goals")
		bool bDisableNewGoals;

	// The min delay at start or after a goal completion before a new goal is made available. 0.0 = no delay. Default=0.0
	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, Category = "Gameplay Goals")
		float DelayBetweenNewGoalsMin;

	// The max delay at start or after a goal completion before a new goal is made available. 0.0 = no delay. Default=0.0
	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, Category = "Gameplay Goals")
		float DelayBetweenNewGoalsMax;

	// The maximum number of active goals for this goal provider component. 0 = no maximum. Default = 0
	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, Category = "Gameplay Goals")
		int32 MaximumCurrentGoals;

protected:
	UPROPERTY(BlueprintReadOnly, BlueprintGetter = "GetGoalProviderGuid", SaveGame, Category = "Gameplay Goals")
		FGuid GameplayGoalProviderGuid;
	
	// Holds the goal names from the data table that could still become available. i.e. not completed nor currently active
	UPROPERTY(BlueprintReadonly, Category = "Gameplay Goals")
		TSet<FName> RemainingGoalNamesCached;

	// Used to decrement on tick for timing new goal creation. This is reset to DelayBetweenNewGoals on a goal completion.
	float CurrentSecondsTillNewGoal;

	int32 CurrentActiveGoals;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void InitGoalCache();

	float GetDelayBetweenNewGoals();

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void BeginDestroy() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay Goals")
		TArray<FGameplayGoal> GetNewGameplayGoals(const TArray<FGameplayGoal>& CurrentGoals, const TArray<FName>& CompletedGoals, const TArray<FName>& AbandonedGoals);
	virtual TArray<FGameplayGoal> GetNewGameplayGoals_Implementation(const TArray<FGameplayGoal>& CurrentGoals, const TArray<FName>& CompletedGoals, const TArray<FName>& AbandonedGoals) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay Goals")
		void OnGameplayGoalCompleted(const FGameplayGoal& CompletedGoal);
	virtual void OnGameplayGoalCompleted_Implementation(const FGameplayGoal& CompletedGoal) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay Goals")
		void OnGameplayGoalAbandoned(const FGameplayGoal& AbandonedGoal);
	virtual void OnGameplayGoalAbandoned_Implementation(const FGameplayGoal& AbandonedGoal) override;

	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay Goals")
	//	FGameplayGoal GetNewRandomGameplayGoal(const TArray<FGameplayGoal>& CurrentGoals, const TArray<FName>& CompletedGoals, const int MinTier = 0, const int MaxTier = 99);
	//virtual FGameplayGoal GetNewRandomGameplayGoal_Implementation(const TArray<FGameplayGoal>& CurrentGoals, const TArray<FName>& CompletedGoals, const int MinTier = 0, const int MaxTier = 99) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay Goals")
		FGuid GetGameplayGoalProviderGuid();
	virtual FGuid GetGameplayGoalProviderGuid_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay Goals")
		FName GetGameplayGoalProviderUniqueName();
	virtual FName GetGameplayGoalProviderUniqueName_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay Goals")
		FString GetGameplayGoalProviderFriendlyName();
	virtual FString GetGameplayGoalProviderFriendlyName_Implementation() override;

	// For access not via interface
	UFUNCTION(BlueprintGetter, Category = "Gameplay Goals")
		FORCEINLINE FGuid GetGoalProviderGuid() { return GetGameplayGoalProviderGuid(); }
};
