// Fill out your copyright notice in the Description page of Project Settings.


#include "GoalsProviderComponent.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UGoalsProviderComponent::UGoalsProviderComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;

	// ...
	bDisableNewGoals = false;
	MaximumCurrentGoals = 0.0;
	CurrentSecondsTillNewGoal = 0.0;
	DelayBetweenNewGoalsMin = 0.0;
	DelayBetweenNewGoalsMax = 0.0;
	if (!GameplayGoalProviderGuid.IsValid())
	{
		GameplayGoalProviderGuid = FGuid::NewGuid();
	}
}

// Called when the game starts
void UGoalsProviderComponent::BeginPlay()
{
	Super::BeginPlay();

	InitGoalCache();
	if (CurrentSecondsTillNewGoal == 0.0)
	{
		CurrentSecondsTillNewGoal = GetDelayBetweenNewGoals();
	}
	if (CurrentSecondsTillNewGoal > 0.0)
	{
		SetComponentTickEnabled(true);
	}
}

void UGoalsProviderComponent::InitGoalCache()
{
	UE_LOG(LogFFGame, Verbose, TEXT("%s InitGoalCache"), *GetNameSafe(this));
	if (GoalsData)
	{
		TArray<FName> AllNames = GoalsData->GetRowNames();
		RemainingGoalNamesCached.Empty(AllNames.Num());
		for (FName GoalName : AllNames)
		{
			RemainingGoalNamesCached.Add(GoalName);
		}
		UE_LOG(LogFFGame, Verbose, TEXT("  Found %d goals"), RemainingGoalNamesCached.Num());
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


float UGoalsProviderComponent::GetDelayBetweenNewGoals()
{
	if (DelayBetweenNewGoalsMin == DelayBetweenNewGoalsMax) {
		return DelayBetweenNewGoalsMax;
	}
	return FMath::RandRange(DelayBetweenNewGoalsMin, DelayBetweenNewGoalsMax);
}


// Called every frame
void UGoalsProviderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Handle new goal timing
	// If we have remaining wait time, deduct delta time from remaining.
	if (CurrentSecondsTillNewGoal > 0.0 && DeltaTime > 0.0)
	{
		CurrentSecondsTillNewGoal -= DeltaTime;
		if (CurrentSecondsTillNewGoal < 0)
		{
			CurrentSecondsTillNewGoal = 0;
		}
	}
	
	if (CurrentSecondsTillNewGoal == 0)
	{
		// If wait time is 0 make sure we allow new goals
		if (bDisableNewGoals)
		{
			UE_LOG(LogFFGame, Verbose, TEXT("GoalProviderComponent wait time complete. Enabling new goals."))
			bDisableNewGoals = false;
			OnNewGoalsEnabled.Broadcast();	
		}
		SetComponentTickEnabled(false);
	}
	else
	{
		// If wait time > 0 make sure we disable new goals
		if (!bDisableNewGoals)
		{
			UE_LOG(LogFFGame, Verbose, TEXT("GoalProviderComponent wait time disabling new goals."))
			bDisableNewGoals = true;
		}
	}
}

void UGoalsProviderComponent::BeginDestroy()
{
	Super::BeginDestroy();

	RemainingGoalNamesCached.Empty();
}

TArray<FGameplayGoal> UGoalsProviderComponent::GetNewGameplayGoals_Implementation(const TArray<FGameplayGoal>& CurrentGoals, const TArray<FName>& CompletedGoals, const TArray<FName>& AbandonedGoals)
{
	TArray<FGameplayGoal> NewGoals; 
	NewGoals.Empty();
	
	if (bDisableNewGoals) 
	{
		UE_LOG(LogFFGame, Verbose, TEXT("%s GetNewGameplayGoals %s New Goals DISABLED"), *GetNameSafe(this), (GetOwner() == nullptr ? TEXT("Unattached") : *GetNameSafe(GetOwner())));
		return NewGoals; 
	}
	if (MaximumCurrentGoals > 0.0 && CurrentActiveGoals >= MaximumCurrentGoals)
	{
		UE_LOG(LogFFGame, Verbose, TEXT("%s GetNewGameplayGoals %s at maximum current goals %d"), *GetNameSafe(this), (GetOwner() == nullptr ? TEXT("Unattached") : *GetNameSafe(GetOwner())), CurrentActiveGoals);
		return NewGoals;
	}
	
	/*
	UE_LOG(LogFFGame, Warning, TEXT("%s GetNewGameplayGoals %s"), *GetNameSafe(this), (GetOwner() == nullptr ? TEXT("Unattached") : *GetNameSafe(GetOwner())) );
	UE_LOG(LogFFGame, Warning, TEXT("  CurrentGoals:"));
	for (const FGameplayGoal& TmpGoal : CurrentGoals)
	{
		UE_LOG(LogFFGame, Warning, TEXT("   %s"), *TmpGoal.UniqueName.ToString());
	}
	UE_LOG(LogFFGame, Warning, TEXT("  CompletedGoals:"));
	for (const FName& TmpGoal : CompletedGoals)
	{
		UE_LOG(LogFFGame, Warning, TEXT("   %s  index: %d  number: %d"), *TmpGoal.ToString(), TmpGoal.GetComparisonIndex(), TmpGoal.GetNumber());
	}
	*/
		
	const FString ContextString("Goals Provider Component");
	FGameplayGoal* CurGoal = nullptr;
	bool bAddToNew = true;
	TSet<FName> ToRemoveFromRemaining;
				
	for (FName GoalName : RemainingGoalNamesCached)
	{
		// Go to next if this one is aready currently active (don't allow repeat here)
		if (CurrentGoals.Contains(GoalName)) 
		{ 
			continue; 
		}
		// Make sure we don't exceed maxium goals
		if (MaximumCurrentGoals == 0 || CurrentActiveGoals < MaximumCurrentGoals)
		{
			bAddToNew = true;
			CurGoal = GoalsData->FindRow<FGameplayGoal>(GoalName, ContextString);
			if (CurGoal)
			{
				// Go to the next one if this one is already complete and cannot be repeated.
				if (!CurGoal->CanRepeat && (CompletedGoals.Contains(GoalName) || AbandonedGoals.Contains(GoalName)))
				{
					// Also remove it from our remaining goals cache
					ToRemoveFromRemaining.Add(GoalName);
					continue;
				}
				// Check each goal's prerequisites
				if (CurGoal && CurGoal->PrerequisiteGoals.Num() > 0)
				{
					for (FName PrereqName : CurGoal->PrerequisiteGoals)
					{
						if (!CompletedGoals.Contains(PrereqName))
						{
							// If we don't have all pre-reqs, then don't add
							bAddToNew = false;
							// And stop checking this goal's pre-reqs
							break;
						}
					}
				}
				if (bAddToNew)
				{
					NewGoals.AddUnique(*CurGoal);
					++CurrentActiveGoals;
				}
			}
			else
			{
				UE_LOG(LogFFGame, Warning, TEXT("%s GetNewGameplayGoals looking for unknown goal: %s"), *GetNameSafe(this), *GoalName.ToString());
			}
		}
		else
		{
			//UE_LOG(LogFFGame, Verbose, TEXT("%s goal provider hit maxium active goals: %d"), *GetNameSafe(this), CurrentActiveGoals);
			break;
		}
	}

	// Remove from our available goals cache
	for (FName RemoveName : ToRemoveFromRemaining)
	{
		RemainingGoalNamesCached.Remove(RemoveName);
	}

	UE_LOG(LogFFGame, Verbose, TEXT("  New Goals: %d"), NewGoals.Num());
	
	return NewGoals;
}

void UGoalsProviderComponent::OnGameplayGoalCompleted_Implementation(const FGameplayGoal & CompletedGoal)
{
	// Double-check the provider GUID
	if (CompletedGoal.ProviderGuid == GameplayGoalProviderGuid)
	{
		if (!CompletedGoal.CanRepeat)
		{
			// remove it from our remaining goals cache
			RemainingGoalNamesCached.Remove(CompletedGoal.UniqueName);
		}
		if (CurrentActiveGoals > 0)
		{
			--CurrentActiveGoals;
		}
		CurrentSecondsTillNewGoal = GetDelayBetweenNewGoals();
		if (CurrentSecondsTillNewGoal > 0.0)
		{
			SetComponentTickEnabled(true);
		}
	}
}

void UGoalsProviderComponent::OnGameplayGoalAbandoned_Implementation(const FGameplayGoal & AbandonedGoal)
{
	// Double-check the provider GUID
	if (AbandonedGoal.ProviderGuid == GameplayGoalProviderGuid)
	{
		if (AbandonedGoal.CanAbandon)
		{
			if (!AbandonedGoal.CanRepeat)
			{
				// remove it from our remaining goals cache
				RemainingGoalNamesCached.Remove(AbandonedGoal.UniqueName);
			}
			if (CurrentActiveGoals > 0)
			{
				--CurrentActiveGoals;
			}
		}
		else
		{
			UE_LOG(LogFFGame, Warning, TEXT("%s Attempting Abandon of non-abandonable goal: %s"), *GetNameSafe(this), *AbandonedGoal.UniqueName.ToString())
		}
	}
}

//FGameplayGoal UGoalsProviderComponent::GetNewRandomGameplayGoal_Implementation(const TArray<FGameplayGoal>& CurrentGoals, const TArray<FName>& CompletedGoals, const int MinTier, const int MaxTier)
//{
//	// Subclass should implement meaningful logic
//	return FGameplayGoal();
//}

FGuid UGoalsProviderComponent::GetGameplayGoalProviderGuid_Implementation()
{
	// Lazy init
	if (!GameplayGoalProviderGuid.IsValid())
	{
		GameplayGoalProviderGuid = FGuid::NewGuid();
	}
	return GameplayGoalProviderGuid;
}

FName UGoalsProviderComponent::GetGameplayGoalProviderUniqueName_Implementation()
{
	return UniqueName;
}


FString UGoalsProviderComponent::GetGameplayGoalProviderFriendlyName_Implementation()
{
	return FriendlyName;
}

