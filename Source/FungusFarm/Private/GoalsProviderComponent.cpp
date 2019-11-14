// Fill out your copyright notice in the Description page of Project Settings.


#include "GoalsProviderComponent.h"
#include "GoodsFunctionLibrary.h"
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
	MaximumCurrentGoals = 0.0f;
	CurrentSecondsTillNewGoal = 0.0f;
	DelayBetweenNewGoalsMin = 0.0f;
	DelayBetweenNewGoalsMax = 0.0f;
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
	if (CurrentSecondsTillNewGoal <= 0.0f)
	{
		ResetTimerBetweenGoals();
	}
}


void UGoalsProviderComponent::InitGoalCache()
{
	if (GoalsData)
	{
		TArray<FName> AllNames = GoalsData->GetRowNames();
		RemainingGoalNamesCached.Empty(AllNames.Num());
		bool bUseTemplate = GoalsData->GetRowStruct() == FGameplayGoalTemplate::StaticStruct();

		for (FName GoalName : AllNames)
		{
			if (bUseTemplate)
			{
				FGameplayGoalTemplate* GoalTemplate = GoalsData->FindRow<FGameplayGoalTemplate>(GoalName, "GoalProviderComponent::InitGoalCache");
				if (GoalTemplate)
				{
					RemainingGoalNamesCached.Add(GoalTemplate->UniqueNameBase);
				}
			}
			else
			{
				FGameplayGoal* Goal = GoalsData->FindRow<FGameplayGoal>(GoalName, "GoalProviderComponent::InitGoalCache");
				if (Goal)
				{
					RemainingGoalNamesCached.Add(Goal->UniqueName);
				}
			}						
		}
		UE_LOG(LogFFGame, Verbose, TEXT("% init found %d goals"), *GetNameSafe(this), RemainingGoalNamesCached.Num());
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


// Returns true if the time was reset.
bool UGoalsProviderComponent::ResetTimerBetweenGoals()
{
	if (CurrentSecondsTillNewGoal <= 0.0f)
	{
		CurrentSecondsTillNewGoal = GetDelayBetweenNewGoals();
	}
	if (CurrentSecondsTillNewGoal > 0.0f)
	{
		UE_LOG(LogFFGame, Log, TEXT("%s reset goals timer %f"), *GetNameSafe(this), CurrentSecondsTillNewGoal);
		bDisableNewGoals = true;
		SetComponentTickEnabled(true);
		return true;
	}
	return false;
}


FGameplayGoal UGoalsProviderComponent::GoalFromTemplate(const FGameplayGoalTemplate& GoalTemplate)
{
	FGameplayGoal NewGoal = FGameplayGoal();
	if (!GoalTemplate.UniqueNameBase.IsNone())
	{
		float Scale = FMath::FRand();
		NewGoal.UniqueName = GoalTemplate.UniqueNameBase;  //.ToString()+"_" + FGuid::NewGuid().ToString()));
		NewGoal.DisplayName = GoalTemplate.DisplayNameBase;
		NewGoal.CanAbandon = GoalTemplate.CanAbandon;
		NewGoal.CanRepeat = GoalTemplate.CanRepeat;
		NewGoal.Hidden = GoalTemplate.Hidden;
		NewGoal.OnAvailableMessage = GoalTemplate.OnAvailableMessage;
		NewGoal.CompletedMessage = GoalTemplate.CompletedMessage;
		NewGoal.PrerequisiteGoals = GoalTemplate.PrerequisiteGoals;
		NewGoal.RequiredExperienceLevel = GoalTemplate.RequiredExperienceLevel;
		NewGoal.HarvestedGoodsToComplete = UGoodsFunctionLibrary::GoodsQuantitiesFromRanges(GoalTemplate.HarvestedGoodsToComplete, Scale);
		NewGoal.SoldGoodsToComplete = UGoodsFunctionLibrary::GoodsQuantitiesFromRanges(GoalTemplate.SoldGoodsToComplete, Scale);
		NewGoal.DonatedGoodsToComplete = UGoodsFunctionLibrary::GoodsQuantitiesFromRanges(GoalTemplate.DonatedGoodsToComplete, Scale);
		NewGoal.CraftedRecipesToComplete = UGoodsFunctionLibrary::NamedQuantitiesToCountMap<FRecipeQuantity>(UGoodsFunctionLibrary::RecipeQuantitiesFromRanges(GoalTemplate.CraftedRecipesToComplete, Scale));
		NewGoal.UnlockedCraftingRecipes = GoalTemplate.UnlockedCraftingRecipes;
		NewGoal.UnlockedCrops = GoalTemplate.UnlockedCrops;
		NewGoal.UnlockedTools = GoalTemplate.UnlockedTools;
		NewGoal.OtherAwards = GoalTemplate.OtherAwards;
		NewGoal.GoodsAwarded = UGoodsFunctionLibrary::FlattenToEvaluatedGoods(GoalTemplate.GoodsAwarded);
		NewGoal.ExperienceAwarded = FMath::TruncToFloat(GoalTemplate.ExperienceAwardedMinimum + (Scale * (GoalTemplate.ExperienceAwardedMaximum - GoalTemplate.ExperienceAwardedMinimum)));
		NewGoal.ProviderGuid = GameplayGoalProviderGuid;
	}
	return NewGoal;
}

TArray<FGameplayGoal> UGoalsProviderComponent::NewGoalsByGoalData(const TArray<FGameplayGoal>& CurrentGoals, const TArray<FName>& CompletedGoals, const TArray<FName>& AbandonedGoals, const float CurrentExperienceLevel)
{
	TArray<FGameplayGoal> NewGoals;
	const FString ContextString("Goals Provider Component");
	FGameplayGoal* NewGoal = nullptr;
	bool bAddToNew = true;
	TSet<FName> ToRemoveFromRemaining;

	// Find all available goal possibilities
	for (FName GoalName : RemainingGoalNamesCached)
	{
		// Go to next if this one is aready currently active (don't allow repeat here)
		if (CurrentGoals.Contains(GoalName))
		{
			continue;
		}

		bAddToNew = true;
		
		if (MaximumCurrentGoals > 0 && CurrentActiveGoals >= MaximumCurrentGoals)
		{
			// Stop looking for goals if we are at the maximum
			break;
		}
		NewGoal = GoalsData->FindRow<FGameplayGoal>(GoalName, ContextString);
		if (NewGoal)
		{
			// Go to the next one if this one is already complete and cannot be repeated.
			if (!NewGoal->CanRepeat && (CompletedGoals.Contains(GoalName) || AbandonedGoals.Contains(GoalName)))
			{
				// Also remove it from our remaining goals cache
				ToRemoveFromRemaining.Add(GoalName);
				continue;
			}
			// Go to the next goal if minimum experience level isn't met
			if (NewGoal->RequiredExperienceLevel > 0 && NewGoal->RequiredExperienceLevel > CurrentExperienceLevel)
			{
				continue;
			}
			// Check each goal's prerequisites
			if (NewGoal->PrerequisiteGoals.Num() > 0)
			{
				for (FName PrereqName : NewGoal->PrerequisiteGoals)
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
				NewGoals.AddUnique(*NewGoal);
				++CurrentActiveGoals;
				// If we delay between goals, stop looking for more.
				if (ResetTimerBetweenGoals()) { break; }
			}
		}
		else
		{
			UE_LOG(LogFFGame, Warning, TEXT("%s GetNewGameplayGoals looking for unknown goal: %s"), *GetNameSafe(this), *GoalName.ToString());
		}
	}

	// Remove from our available goals cache
	for (FName RemoveName : ToRemoveFromRemaining)
	{
		RemainingGoalNamesCached.Remove(RemoveName);
	}

	return NewGoals;
}

TArray<FGameplayGoal> UGoalsProviderComponent::NewGoalsByTemplate(const TArray<FGameplayGoal>& CurrentGoals, const TArray<FName>& CompletedGoals, const TArray<FName>& AbandonedGoals, const float CurrentExperienceLevel)
{
	bool bAddToNew;
	TArray<FGameplayGoalTemplate> NewGoalPossibilities;
	TArray<FGameplayGoal> NewGoals;
	FGameplayGoalTemplate* GoalTemplate = nullptr;
	TSet<FName> ToRemoveFromRemaining;
	float TotalWeightedChance = 0.0f;

	// Find all available goal possibilities
	for (FName GoalName : RemainingGoalNamesCached)
	{
		// Go to next if this one is aready currently active (don't allow repeat here)
		if (CurrentGoals.Contains(GoalName))
		{
			continue;
		}

		bAddToNew = true;
		// Get goal from template
		if (GoalsData->GetRowStruct() == FGameplayGoalTemplate::StaticStruct())
		{
			GoalTemplate = GoalsData->FindRow<FGameplayGoalTemplate>(GoalName, "GoalsProviderComponent");
			if (GoalTemplate && !GoalTemplate->UniqueNameBase.IsNone())
			{
				// Go to the next one if this one is already complete and cannot be repeated.
				if (!GoalTemplate->CanRepeat && (CompletedGoals.Contains(GoalName) || AbandonedGoals.Contains(GoalName)))
				{
					// Also remove it from our remaining goals cache
					ToRemoveFromRemaining.Add(GoalName);
					continue;
				}
				// Go to the next goal if minimum experience level isn't met
				if (GoalTemplate->RequiredExperienceLevel > 0 && GoalTemplate->RequiredExperienceLevel > CurrentExperienceLevel)
				{
					continue;
				}
				// Check each goal's prerequisites
				if (GoalTemplate && GoalTemplate->PrerequisiteGoals.Num() > 0)
				{
					for (FName PrereqName : GoalTemplate->PrerequisiteGoals)
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
					NewGoalPossibilities.AddUnique(*GoalTemplate);
					TotalWeightedChance += GoalTemplate->WeightedChance;
				}
			}
			else
			{
				UE_LOG(LogFFGame, Warning, TEXT("%s GetNewGameplayGoals looking for unknown goal: %s"), *GetNameSafe(this), *GoalName.ToString());
			}
			UE_LOG(LogFFGame, Warning, TEXT("  Possible options for new goal: %d"), NewGoalPossibilities.Num());
		}
	}
	// Pick from possibilities as long as we have them or stop if we delay between goals
	while (NewGoalPossibilities.Num() > 0 && (MaximumCurrentGoals == 0 || (CurrentActiveGoals < MaximumCurrentGoals)))
	{
		GoalTemplate = UGoodsFunctionLibrary::PickOneFromWeightedList<FGameplayGoalTemplate>(NewGoalPossibilities, TotalWeightedChance);
		if (GoalTemplate)
		{
			FGameplayGoal NewGoal = GoalFromTemplate(*GoalTemplate);
			NewGoals.AddUnique(NewGoal);
			NewGoalPossibilities.RemoveSingle(*GoalTemplate);
			CurrentActiveGoals++;
			// If we delay between goals, stop looking for more.
			if (ResetTimerBetweenGoals()) { break; }
		}
		else
		{
			break;
		}
	}

	// Remove from our available goals cache
	for (FName RemoveName : ToRemoveFromRemaining)
	{
		RemainingGoalNamesCached.Remove(RemoveName);
	}

	return NewGoals;
}


// Called every frame
void UGoalsProviderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Handle new goal timing
	// If we have remaining wait time, deduct delta time from remaining.
	if (CurrentSecondsTillNewGoal > 0.0f && DeltaTime > 0.0f)
	{
		CurrentSecondsTillNewGoal -= DeltaTime;
		if (CurrentSecondsTillNewGoal < 0.0f)
		{
			CurrentSecondsTillNewGoal = 0.0f;
		}
	}
	
	if (CurrentSecondsTillNewGoal <= 0.0f)
	{
		// If wait time is 0 allow new goals and broadcast notification
		if (bDisableNewGoals)
		{
			SetComponentTickEnabled(false);
			UE_LOG(LogFFGame, Log, TEXT("GoalProviderComponent wait time complete. Enabling new goals."))
			bDisableNewGoals = false;
			OnNewGoalsEnabled.Broadcast();				
		}		
	}
	else
	{
		// If wait time > 0 make sure we disable new goals
		if (!bDisableNewGoals)
		{
			UE_LOG(LogFFGame, Log, TEXT("GoalProviderComponent tick disabling new goals because wait time > 0."))
			bDisableNewGoals = true;
		}
	}
}


void UGoalsProviderComponent::BeginDestroy()
{
	Super::BeginDestroy();

	RemainingGoalNamesCached.Empty();
}


TArray<FGameplayGoal> UGoalsProviderComponent::GetNewGameplayGoals_Implementation(const TArray<FGameplayGoal>& CurrentGoals, const TArray<FName>& CompletedGoals, const TArray<FName>& AbandonedGoals, const float CurrentExperienceLevel)
{
	TArray<FGameplayGoalTemplate> NewGoalPossibilities;
	TArray<FGameplayGoal> NewGoals;
	const FString ContextString("Goals Provider Component");
	FGameplayGoalTemplate* GoalTemplate = nullptr;
	FGameplayGoal* NewGoal = nullptr;
	float TotalWeightedChance = 0.0f;
	bool bAddToNew = true;
	TSet<FName> ToRemoveFromRemaining;

	if (bDisableNewGoals)
	{
		UE_LOG(LogFFGame, Log, TEXT("%s GetNewGameplayGoals %s New Goals DISABLED"), *GetNameSafe(this), (GetOwner() == nullptr ? TEXT("Unattached") : *GetNameSafe(GetOwner())));
		return NewGoals;
	}
	if (MaximumCurrentGoals > 0 && CurrentActiveGoals >= MaximumCurrentGoals)
	{
		UE_LOG(LogFFGame, Log, TEXT("%s GetNewGameplayGoals %s at maximum current goals %d"), *GetNameSafe(this), (GetOwner() == nullptr ? TEXT("Unattached") : *GetNameSafe(GetOwner())), CurrentActiveGoals);
		return NewGoals;
	}

	UE_LOG(LogFFGame, Log, TEXT("%s GetNewGameplayGoals %s"), *GetNameSafe(this), (GetOwner() == nullptr ? TEXT("Unattached") : *GetNameSafe(GetOwner())));
	/*
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

	if (GoalsData->GetRowStruct() == FGameplayGoalTemplate::StaticStruct())
	{
		NewGoals = NewGoalsByTemplate(CurrentGoals, CompletedGoals, AbandonedGoals, CurrentExperienceLevel);
	}
	else
	{
		NewGoals = NewGoalsByGoalData(CurrentGoals, CompletedGoals, AbandonedGoals, CurrentExperienceLevel);
	}
	   	
	UE_LOG(LogFFGame, Log, TEXT("  New Goals: %d"), NewGoals.Num());
	
	return NewGoals;
}


void UGoalsProviderComponent::OnGameplayGoalCompleted_Implementation(const FGameplayGoal & CompletedGoal)
{
	// Double-check the provider GUID
	if (CompletedGoal.ProviderGuid == GameplayGoalProviderGuid)
	{
		if (!CompletedGoal.CanRepeat)
		{
			// Remove it from our remaining goals cache
			RemainingGoalNamesCached.Remove(CompletedGoal.UniqueName);
		}
		if (CurrentActiveGoals > 0)
		{
			--CurrentActiveGoals;
		}
		if (CurrentActiveGoals < MaximumCurrentGoals)
		{
			ResetTimerBetweenGoals();
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
			if (CurrentActiveGoals < MaximumCurrentGoals)
			{
				ResetTimerBetweenGoals();
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

