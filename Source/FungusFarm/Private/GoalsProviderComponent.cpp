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
		if (Scale < 0.001f)
		{
			Scale = 0.001f;
		}
		NewGoal.UniqueName = GoalTemplate.UniqueNameBase;  //.ToString()+"_" + FGuid::NewGuid().ToString()));
		NewGoal.DisplayName = GoalTemplate.DisplayNameBase;
		NewGoal.CanAbandon = GoalTemplate.CanAbandon;
		NewGoal.MaxRepeats = GoalTemplate.MaxRepeats;
		NewGoal.Hidden = GoalTemplate.Hidden;
		NewGoal.OnAvailableMessage = GoalTemplate.OnAvailableMessage;
		NewGoal.CompletedMessage = GoalTemplate.CompletedMessage;
		NewGoal.PrerequisiteGoals = GoalTemplate.PrerequisiteGoals;
		NewGoal.PrerequisiteRecipes = GoalTemplate.PrerequisiteRecipes;
		NewGoal.RequiredExperienceLevel = GoalTemplate.RequiredExperienceLevel;
		NewGoal.HarvestedGoodsToComplete = UGoodsFunctionLibrary::GoodsQuantitiesFromRanges(GoalTemplate.HarvestedGoodsToComplete, Scale);
		NewGoal.SoldGoodsToComplete = UGoodsFunctionLibrary::GoodsQuantitiesFromRanges(GoalTemplate.SoldGoodsToComplete, Scale);
		NewGoal.DonatedGoodsToComplete = UGoodsFunctionLibrary::GoodsQuantitiesFromRanges(GoalTemplate.DonatedGoodsToComplete, Scale);
		NewGoal.CraftedRecipesToComplete = UGoodsFunctionLibrary::NamedQuantitiesToCountMap<FRecipeQuantity>(UGoodsFunctionLibrary::RecipeQuantitiesFromRanges(GoalTemplate.CraftedRecipesToComplete, Scale));
		NewGoal.OverrideImage = GoalTemplate.OverrideImage;
		NewGoal.OverrideCompletedImage = GoalTemplate.OverrideCompletedImage;
		NewGoal.UnlockedCraftingRecipes = GoalTemplate.UnlockedCraftingRecipes;
		NewGoal.UnlockedCrops = GoalTemplate.UnlockedCrops;
		NewGoal.UnlockedTools = GoalTemplate.UnlockedTools;
		NewGoal.OtherAwards = GoalTemplate.OtherAwards;
		NewGoal.GoodsAwarded = UGoodsFunctionLibrary::FlattenToEvaluatedGoods(GoalTemplate.GoodsAwarded, Scale);
		NewGoal.ExperienceAwarded = FMath::TruncToFloat(GoalTemplate.ExperienceAwardedMinimum + (Scale * (GoalTemplate.ExperienceAwardedMaximum - GoalTemplate.ExperienceAwardedMinimum)));
		NewGoal.ProviderGuid = GameplayGoalProviderGuid;
	}
	return NewGoal;
}


TArray<FGameplayGoal> UGoalsProviderComponent::NewGoalsByGoalData(const TArray<FGameplayGoal>& CurrentGoals, const TArray<FNameCountInt>& CompletedGoals, const TArray<FName>& AbandonedGoals, const TArray<FName>& UnlockedRecipes, const float CurrentExperienceLevel)
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
			const FNameCountInt* CompletedCountStruct = CompletedGoals.FindByKey(GoalName);
			if (CompletedCountStruct)
			{
				// Go to the next one if this one is already completed max times and cannot be repeated.
				if (NewGoal->MaxRepeats != -1 && CompletedCountStruct->Count > (NewGoal->MaxRepeats - 1))
				{
					// Also remove it from our remaining goals cache
					ToRemoveFromRemaining.Add(GoalName);
					continue;
				}
			}
			// Go to the next goal if minimum experience level isn't met
			if (NewGoal->RequiredExperienceLevel > 0 && NewGoal->RequiredExperienceLevel > CurrentExperienceLevel)
			{
				continue;
			}
			// Check each goal prerequisite
			if (NewGoal->PrerequisiteGoals.Num() > 0)
			{
				for (FName PrereqName : NewGoal->PrerequisiteGoals)
				{
					if (!CompletedGoals.Contains(PrereqName))
					{
						// If we don't have all pre-reqs, then don't add to new
						bAddToNew = false;
						// And stop checking this goal's pre-reqs
						break;
					}
				}
			}
			// Check each goal prerequisite recipes
			if (NewGoal->PrerequisiteRecipes.Num() > 0)
			{
				for (FName PrereqName : NewGoal->PrerequisiteRecipes)
				{
					if (!UnlockedRecipes.Contains(PrereqName))
					{
						// If we don't have all pre-reqs, then don't add to new
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
 

TArray<FGameplayGoal> UGoalsProviderComponent::NewGoalsByTemplate(const TArray<FGameplayGoal>& CurrentGoals, const TArray<FNameCountInt>& CompletedGoals, const TArray<FName>& AbandonedGoals, const TArray<FName>& UnlockedRecipes, const float CurrentExperienceLevel)
{
	bool bAddToNew;
	TArray<FGameplayGoalTemplate> NewGoalPossibilities;
	TArray<FGameplayGoal> NewGoals;
	FGameplayGoalTemplate* GoalTemplate = nullptr;
	TSet<FName> ToRemoveFromRemaining;
	float TotalWeightedChance = 0.0f;
	const FNameCountInt* CompletedCountStruct = nullptr;

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
				CompletedCountStruct = CompletedGoals.FindByKey(GoalName);
				if (CompletedCountStruct)
				{
					// Go to the next one if this one is already completed max times and cannot be repeated.
					if (GoalTemplate->MaxRepeats != -1 && CompletedCountStruct->Count > (GoalTemplate->MaxRepeats - 1))
					{
						// Also remove it from our remaining goals cache
						ToRemoveFromRemaining.Add(GoalName);
						continue;
					}
				}
				// Go to the next goal if minimum experience level isn't met
				if (GoalTemplate->RequiredExperienceLevel > 0.0f && GoalTemplate->RequiredExperienceLevel > CurrentExperienceLevel)
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
				// Check each goal prerequisite recipes
				if (GoalTemplate->PrerequisiteRecipes.Num() > 0)
				{
					for (FName PrereqName : GoalTemplate->PrerequisiteRecipes)
					{
						if (!UnlockedRecipes.Contains(PrereqName))
						{
							// If we don't have all pre-reqs, then don't add to new
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
	while (NewGoalPossibilities.Num() > 0 && (MaximumCurrentGoals <= 0 || (CurrentActiveGoals < MaximumCurrentGoals)))
	{
		GoalTemplate = UGoodsFunctionLibrary::PickOneFromWeightedList<FGameplayGoalTemplate>(NewGoalPossibilities, TotalWeightedChance);
		if (GoalTemplate)
		{
			FGameplayGoal NewGoal = GoalFromTemplate(*GoalTemplate);
			NewGoals.AddUnique(NewGoal);
			NewGoalPossibilities.RemoveSingle(*GoalTemplate);
			TotalWeightedChance -= GoalTemplate->WeightedChance;
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


TArray<FGameplayGoal> UGoalsProviderComponent::GetNewGameplayGoals_Implementation(const TArray<FGameplayGoal>& CurrentGoals, const TArray<FNameCountInt>& CompletedGoals, const TArray<FName>& AbandonedGoals, const TArray<FName>& UnlockedRecipes, const float CurrentExperienceLevel)
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
	if (MaximumCurrentGoals > 0.0f && CurrentActiveGoals >= MaximumCurrentGoals)
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
	for (const FNameCountInt& TmpGoal : CompletedGoals)
	{
		UE_LOG(LogFFGame, Warning, TEXT("   %s  index: %d  number: %d"), *TmpGoal.Name.ToString(), TmpGoal.GetComparisonIndex(), TmpGoal.GetNumber());
	}
	*/

	// Get goals based on our GoalsData type - either a GoalTemplate or a Goal.
	if (GoalsData->GetRowStruct() == FGameplayGoalTemplate::StaticStruct())
	{
		NewGoals = NewGoalsByTemplate(CurrentGoals, CompletedGoals, AbandonedGoals, UnlockedRecipes, CurrentExperienceLevel);
	}
	else if (GoalsData->GetRowStruct() == FGameplayGoal::StaticStruct())
	{
		NewGoals = NewGoalsByGoalData(CurrentGoals, CompletedGoals, AbandonedGoals, UnlockedRecipes, CurrentExperienceLevel);
	}
	else
	{
		UE_LOG(LogFFGame, Warning, TEXT("%s GetNewGameplayGoal - Unknown structure in goals data file."), *GetNameSafe(this));
		return NewGoals;
	}
	   	
	UE_LOG(LogFFGame, Log, TEXT("  New Goals: %d"), NewGoals.Num());
	
	return NewGoals;
}


void UGoalsProviderComponent::OnGameplayGoalCompleted_Implementation(const FGameplayGoal & CompletedGoal, const int32 CurrentGoalCompletions)
{
	// Double-check the provider GUID
	if (CompletedGoal.ProviderGuid == GameplayGoalProviderGuid)
	{
		if (CompletedGoal.MaxRepeats != -1 && CompletedGoal.MaxRepeats >= CurrentGoalCompletions)
		{
			// Remove it from our remaining goals cache
			RemainingGoalNamesCached.Remove(CompletedGoal.UniqueName);
		}
		if (CurrentActiveGoals > 0)
		{
			--CurrentActiveGoals;
		}
		if (MaximumCurrentGoals <= 0.0f || CurrentActiveGoals < MaximumCurrentGoals)
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
			/*
			if (AbandonedGoal.MaxRepeats == 0 || AbandonedGoal.MaxRepeats == 1)
			{
				// remove it from our remaining goals cache
				RemainingGoalNamesCached.Remove(AbandonedGoal.UniqueName);
			}
			*/
			if (CurrentActiveGoals > 0)
			{
				--CurrentActiveGoals;
			}
			if (MaximumCurrentGoals <= 0.0f || CurrentActiveGoals < MaximumCurrentGoals)
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

TAssetPtr<UTexture2D> UGoalsProviderComponent::GetGameplayGoalProviderImage_Implementation()
{
	return ProviderImage;
}
