// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorGoalsComponent.h"
#include "FFGameMode.h"
#include "FFPlayerControllerBase.h"

// Sets default values for this component's properties
UActorGoalsComponent::UActorGoalsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UActorGoalsComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

bool UActorGoalsComponent::CheckGoalQualifiesComplete(const FGameplayGoal & Goal)
{
	// Harvest totals
	for (const FGoodsQuantity& RequiredGoods : Goal.HarvestedGoodsToComplete) 
	{
		if (Goal.HarvestedGoodsProgress.FindRef(RequiredGoods.Name) < RequiredGoods.Quantity) { return false; }
	}

	// Sold goods
	for (const FGoodsQuantity& RequiredGoods : Goal.SoldGoodsToComplete)
	{
		if (Goal.SoldGoodsProgress.FindRef(RequiredGoods.Name) < RequiredGoods.Quantity) { return false; }
	}

	// Donated goods
	for (const FGoodsQuantity& RequiredGoods : Goal.DonatedGoodsToComplete)
	{
		if (Goal.DonatedGoodsProgress.FindRef(RequiredGoods.Name) < RequiredGoods.Quantity) { return false; }
	}

	// Recipes crafted
	for (auto& RequiredRecipe : Goal.CraftedRecipesToComplete)
	{
		if (Goal.CraftedRecipesProgress.FindRef(RequiredRecipe.Key) < RequiredRecipe.Value) { return false; }
	}
	
	return true;
}

void UActorGoalsComponent::InitGoalProviders()
{
	if (GoalProviders.Num() == 0)
	{
		// Use GameMode as default provider
		AFFGameMode * _GameMode = Cast<AFFGameMode>(GetWorld()->GetAuthGameMode());
		if (_GameMode)
		{
			AddGoalProvider(_GameMode);
		}
	}
}


// Called every frame
void UActorGoalsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// Add a new GameplayGoalsProvider 
void UActorGoalsComponent::AddGoalProvider(const TScriptInterface<IGameplayGoalProvider>& NewProvider)
{
	if (NewProvider)
	{
		GoalProviders.AddUnique(NewProvider);
	}
}

// Remove the given GameplayGoalProvider from this component
int32 UActorGoalsComponent::RemoveGoalProvider(const TScriptInterface<IGameplayGoalProvider>& ToRemoveProvider)
{
	if (ToRemoveProvider)
	{
		return GoalProviders.Remove(ToRemoveProvider);
	}
	return 0;
}

// Check all Goal Providers for newly available goals.
void UActorGoalsComponent::CheckForNewGoals()
{
	InitGoalProviders();
	if (GoalProviders.Num() > 0) {
		TArray<FGameplayGoal> NewGoals;
		// Get any new goals from all goal providers
		for (TScriptInterface<IGameplayGoalProvider> Provider : GoalProviders)
		{ 
			FGuid ProviderGuid = IGameplayGoalProvider::Execute_GetGameplayGoalProviderGuid(Provider.GetObject());
			TArray<FGameplayGoal> TmpGoals = IGameplayGoalProvider::Execute_GetNewGameplayGoals(Provider.GetObject(), CurrentGoals, CompletedGoals);
			if (TmpGoals.Num() > 0) 
			{ 
				// Make sure all goals have correct provider GUID. (don't rely on provider)
				for (FGameplayGoal& TmpGoal : TmpGoals)
				{
					TmpGoal.ProviderGuid = ProviderGuid;
				}
				NewGoals.Append(TmpGoals); 
			}
		}
		if (NewGoals.Num() > 0)
		{
			CurrentGoals.Append(NewGoals);
			// Notify listener
			IGameplayGoalListener * OwnerListener = Cast<IGameplayGoalListener>(GetOwner());
			if (OwnerListener)
			{
				IGameplayGoalListener::Execute_OnNewGameplayGoals(GetOwner(), NewGoals);
			}
			//UE_LOG(LogTemp, Warning, TEXT("New Goals: %d"), NewGoals.Num());
		}
	}
}

// Ask our Goal Providers for a new random goal.
// Stops with first provider to provide a valid goal.
void UActorGoalsComponent::CreateNewRandomGoal(const int MinTier, const int MaxTier, bool& bSuccess, FGameplayGoal& NewRandomGoal)
{
	InitGoalProviders();
	if (GoalProviders.Num() > 0) 
	{
		FGameplayGoal NewGoal;
		for (TScriptInterface<IGameplayGoalProvider> Provider : GoalProviders)
		{
			NewGoal = IGameplayGoalProvider::Execute_GetNewRandomGameplayGoal(Provider.GetObject(), CurrentGoals, CompletedGoals, MinTier, MaxTier);
			// Return first valid one
			if (!NewGoal.UniqueName.IsNone())
			{
				NewRandomGoal = NewGoal;
				bSuccess = true;
				return;
			}
		}		
	}
	bSuccess = false;
	return;
}

// Check to see if any of our current goals qualify as completed.
// Will set them complete if so. (and call resulting events/notifications)
void UActorGoalsComponent::CheckForCompletedGoals()
{
	TArray<FGameplayGoal> NewlyCompletedGoals;
	for (FGameplayGoal& CurGoal : CurrentGoals)
	{
		if (CheckGoalQualifiesComplete(CurGoal)) { NewlyCompletedGoals.Add(CurGoal); }
	}

	SetGoalsComplete(NewlyCompletedGoals, bCheckForNewOnComplete);
}

// Add a goal name to the list of completed goals.
// if bCheckForNewGoals=true then this will also trigger a "new goals" check after marking goals complete.
void UActorGoalsComponent::SetGoalsComplete(const TArray<FGameplayGoal>& Goals, const bool bCheckForNewGoals)
{
	if (Goals.Num() > 0)
	{
		for (const FGameplayGoal& CurGoal : Goals)
		{
			CurrentGoals.RemoveSingle(CurGoal);
			CompletedGoals.AddUnique(CurGoal.UniqueName);
		}
		// Notify listener of completed goals
		IGameplayGoalListener * OwnerListener = Cast<IGameplayGoalListener>(GetOwner());
		if (OwnerListener) { IGameplayGoalListener::Execute_OnGameplayGoalsCompleted(GetOwner(), Goals); }

		if (bCheckForNewGoals) {
			CheckForNewGoals(); 
		}
	}
}

void UActorGoalsComponent::UpdateHarvestedGoodsProgress(const TArray<FGoodsQuantity>& HarvestedGoods)
{
	if (HarvestedGoods.Num() == 0) { return; }
	bool bAnyUpdated = false;
	TArray<FGameplayGoal> ChangedGoals;

	for (FGameplayGoal& CurGoal : CurrentGoals)
	{
		if (CurGoal.HarvestedGoodsToComplete.Num() > 0) 
		{
			// Initialize harvested goods progress
			if (CurGoal.HarvestedGoodsProgress.Num() == 0)
			{
				for (FGoodsQuantity& CurGoods : CurGoal.HarvestedGoodsToComplete)
				{
					CurGoal.HarvestedGoodsProgress.Add(CurGoods.Name, 0);
				}
			}

			// Update this goal's progress for each harvested goods
			for (const FGoodsQuantity& CurGoods : HarvestedGoods)
			{
				// If this goal requires this goods type, update the progress
				if (CurGoal.HarvestedGoodsProgress.Contains(CurGoods.Name))
				{
					float CurrentValue = CurGoal.HarvestedGoodsProgress.FindRef(CurGoods.Name);
					//UE_LOG(LogTemp, Warning, TEXT("Goal harvest Goods %s total: %d"), *CurGoods.Name.ToString(), int(CurrentValue));
					CurGoal.HarvestedGoodsProgress.Add(CurGoods.Name, CurrentValue + CurGoods.Quantity);
					//UE_LOG(LogTemp, Warning, TEXT("Goal harvest Goods %s total: %d"), *CurGoods.Name.ToString(), int(CurrentValue + CurGoods.Quantity));
					ChangedGoals.Add(CurGoal);
					bAnyUpdated = true;
				}				
			}
		}
	}

	if (ChangedGoals.Num() > 0) { OnProgressChanged.Broadcast(ChangedGoals); }
	if (bAnyUpdated) { CheckForCompletedGoals(); }	
}

void UActorGoalsComponent::UpdateSoldGoodsProgress(const TArray<FGoodsQuantity>& SoldGoods)
{
	if (SoldGoods.Num() == 0) { return; }
	bool bAnyUpdated = false;
	TArray<FGameplayGoal> ChangedGoals;

	for (FGameplayGoal& CurGoal : CurrentGoals)
	{
		if (CurGoal.SoldGoodsToComplete.Num() > 0)
		{
			// Initialize sold goods progress
			if (CurGoal.SoldGoodsProgress.Num() == 0)
			{
				for (FGoodsQuantity& CurGoods : CurGoal.SoldGoodsToComplete)
				{
					CurGoal.SoldGoodsProgress.Add(CurGoods.Name, 0.0);
				}
			}

			// Update this goal's progress for each sold goods
			for (const FGoodsQuantity& CurGoods : SoldGoods)
			{
				// If this goal requires this goods type, update the progress
				if (CurGoal.SoldGoodsProgress.Contains(CurGoods.Name))
				{
					float CurrentValue = CurGoal.SoldGoodsProgress.FindRef(CurGoods.Name);
					//UE_LOG(LogTemp, Warning, TEXT("Goal sold Goods %s total: %d"), *CurGoods.Name.ToString(), int(CurrentValue + CurGoods.Quantity));
					CurGoal.SoldGoodsProgress.Add(CurGoods.Name, CurrentValue + CurGoods.Quantity);
					ChangedGoals.Add(CurGoal);
					bAnyUpdated = true;
				}
			}
		}
	}

	if (ChangedGoals.Num() > 0) { OnProgressChanged.Broadcast(ChangedGoals); }
	if (bAnyUpdated) { CheckForCompletedGoals(); }
}

// Update the DonatedGoods progress for the current goal with with given name.
// Returns false if no current goal has the specified UniqueName.
bool UActorGoalsComponent::UpdateDonatedGoodsProgress(const TArray<FGoodsQuantity>& DonatedGoods, const FName GoalUniqueName)
{
	bool bAnyUpdated = false;
	bool bFound = false;
	TArray<FGameplayGoal> ChangedGoals;
	FGameplayGoal* MatchingGoal = CurrentGoals.FindByKey(GoalUniqueName);
	//for (FGameplayGoal& CurGoal : CurrentGoals)
	//{
		//if (CurGoal.UniqueName == GoalUniqueName)
		if (MatchingGoal)
		{
			bFound = true;
			if (DonatedGoods.Num() == 0) { return bFound; }
			if (MatchingGoal->DonatedGoodsToComplete.Num() > 0)
			{
				// Initialize Donated goods progress
				if (MatchingGoal->DonatedGoodsProgress.Num() == 0)
				{
					for (FGoodsQuantity& CurGoods : MatchingGoal->DonatedGoodsToComplete)
					{
						MatchingGoal->DonatedGoodsProgress.Add(CurGoods.Name, 0.0);
					}
				}

				// Update this goal's progress for each Donated goods
				for (const FGoodsQuantity& CurGoods : DonatedGoods)
				{
					// If this goal requires this goods type, update the progress
					if (MatchingGoal->DonatedGoodsProgress.Contains(CurGoods.Name))
					{
						float CurrentValue = MatchingGoal->DonatedGoodsProgress.FindRef(CurGoods.Name);
						UE_LOG(LogTemp, Warning, TEXT("Goal Donated Goods %s total: %d"), *CurGoods.Name.ToString(), int(CurrentValue + CurGoods.Quantity));
						MatchingGoal->DonatedGoodsProgress.Add(CurGoods.Name, CurrentValue + CurGoods.Quantity);
						ChangedGoals.Add(*MatchingGoal);
						bAnyUpdated = true;
					}
				}
			}
		}
	//}

	if (ChangedGoals.Num() > 0) { OnProgressChanged.Broadcast(ChangedGoals); }
	if (bAnyUpdated) { CheckForCompletedGoals(); }
	return bFound;
}


void UActorGoalsComponent::UpdateCraftedRecipesProgress(const FName & RecipeName, const int NumberCrafted)
{
	if (RecipeName.IsNone() || NumberCrafted == 0) { return; }
	bool bAnyUpdated = false;
	TArray<FGameplayGoal> ChangedGoals;

	for (FGameplayGoal& CurGoal : CurrentGoals)
	{
		if (CurGoal.CraftedRecipesToComplete.Num() > 0)
		{
			// Initialize recipe crafting progress
			if (CurGoal.CraftedRecipesProgress.Num() == 0)
			{
				for (TPair<FName, int32>& CurRecipe : CurGoal.CraftedRecipesToComplete)
				{
					CurGoal.CraftedRecipesProgress.Add(CurRecipe.Key, 0);
					//UE_LOG(LogTemp, Warning, TEXT("Goal crafted recipe init %s total: %d  target: %d"), *CurRecipe.Key.ToString(), 0, CurGoal.CraftedRecipesToComplete.FindRef(CurRecipe.Key));
				}
			}

			// Update each goal's progress if the goal contains this recipe requirement
			if (CurGoal.CraftedRecipesProgress.Contains(RecipeName))
			{
				int CurrentValue = CurGoal.CraftedRecipesProgress.FindRef(RecipeName);
				//UE_LOG(LogTemp, Warning, TEXT("Goal crafted recipe %s current: %d"), *RecipeName.ToString(), CurrentValue);
				CurGoal.CraftedRecipesProgress.Add(RecipeName, CurrentValue + NumberCrafted); // .Add(RecipeName, CurrentValue + NumberCrafted);
				//UE_LOG(LogTemp, Warning, TEXT("Goal crafted recipe %s new total: %d"), *RecipeName.ToString(), CurGoal.CraftedRecipesProgress.FindRef(RecipeName));
				ChangedGoals.Add(CurGoal);
				bAnyUpdated = true;
			}
		}
	}

	if (ChangedGoals.Num() > 0) { OnProgressChanged.Broadcast(ChangedGoals); }
	if (bAnyUpdated) { CheckForCompletedGoals(); }
}

bool UActorGoalsComponent::GetCurrentGoalData(const FName GoalName, FGameplayGoal& GoalData)
{
	for (FGameplayGoal& CurGoal : CurrentGoals)
	{
		if (CurGoal.UniqueName == GoalName)
		{
			GoalData = CurGoal;
			return true;
		}
	}
	return false;
}

