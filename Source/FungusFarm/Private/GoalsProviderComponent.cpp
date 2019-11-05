// Fill out your copyright notice in the Description page of Project Settings.


#include "GoalsProviderComponent.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UGoalsProviderComponent::UGoalsProviderComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UGoalsProviderComponent::BeginPlay()
{
	Super::BeginPlay();

	InitGoalCache();	
}

void UGoalsProviderComponent::InitGoalCache()
{
	UE_LOG(LogFFGame, Warning, TEXT("%s InitGoalCache"), *GetNameSafe(this));
	if (GoalsData)
	{
		TArray<FName> AllNames = GoalsData->GetRowNames();
		RemainingGoalNamesCached.Empty(AllNames.Num());
		for (FName GoalName : AllNames)
		{
			RemainingGoalNamesCached.Add(GoalName);
		}
	}
	else
	{
		if (GetOwner())
		{
			UE_LOG(LogFFGame, Warning, TEXT("Goal data not found for %s on %s"), *GetNameSafe(this), *GetOwner()->GetHumanReadableName());
		}
		else
		{
			UE_LOG(LogFFGame, Warning, TEXT("Goal data not found for un-attached %s"), *GetNameSafe(this));
		}
	}		
}

// Called every frame
void UGoalsProviderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

TArray<FGameplayGoal> UGoalsProviderComponent::GetNewGameplayGoals_Implementation(const TArray<FGameplayGoal>& CurrentGoals, const TArray<FName>& CompletedGoals)
{
	TArray<FGameplayGoal> NewGoals; 
	const FString ContextString("Goals Provider Component");

	UE_LOG(LogFFGame, Warning, TEXT("%s GetNewGameplayGoals"), *GetNameSafe(this));

	//if (RemainingGoalNamesCached.Num() == 0)
	//{
	//	InitGoalCache();
	//}
	
	FGameplayGoal* CurGoal = nullptr;
	bool bAddToNew = true;
	TSet<FName> ToRemoveFromRemaining;
				
	for (FName GoalName : RemainingGoalNamesCached)
	{
		// Go to next if this one is aready completed or active
		if (CurrentGoals.Contains(GoalName) || CompletedGoals.Contains(GoalName)) 
		{ 
			ToRemoveFromRemaining.Add(GoalName);
			continue; 
		}

		bAddToNew = true;
		CurGoal = GoalsData->FindRow<FGameplayGoal>(GoalName, ContextString);
		// Check each goal' prerequisites
		if (CurGoal && CurGoal->PrerequisiteGoals.Num() > 0) 
		{
			for (FName PrereqName : CurGoal->PrerequisiteGoals)
			{
				// If we don't have all pre-reqs, then don't add, and stop checking this goal's pre-reqs
				if (!CompletedGoals.Contains(PrereqName))
				{
					bAddToNew = false;
					break;
				}
			}
		}

		if (bAddToNew)
		{
			NewGoals.AddUnique(*CurGoal);
		}
	}

	// Remove from our available goals cache
	for (FName RemoveName : ToRemoveFromRemaining)
	{
		RemainingGoalNamesCached.Remove(RemoveName);
	}
	
	return NewGoals;
}

FGameplayGoal UGoalsProviderComponent::GetNewRandomGameplayGoal_Implementation(const TArray<FGameplayGoal>& CurrentGoals, const TArray<FName>& CompletedGoals, const int MinTier, const int MaxTier)
{
	// Subclass should implement meaningful logic
	return FGameplayGoal();
}

FGuid UGoalsProviderComponent::GetGameplayGoalProviderGuid_Implementation()
{
	// Lazy init
	if (!GameplayGoalProviderGuid.IsValid())
	{
		GameplayGoalProviderGuid = FGuid::NewGuid();
	}
	return GameplayGoalProviderGuid;
}

