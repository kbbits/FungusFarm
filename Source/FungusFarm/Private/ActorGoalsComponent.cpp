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
	for (FGoodsQuantity RequiredGoods : Goal.HarvestedGoodsToComplete) 
	{
		if (Goal.HarvestedGoodsProgress.FindRef(RequiredGoods.Name) < RequiredGoods.Quantity) { return false; }
	}

	// Sold goods
	for (FGoodsQuantity RequiredGoods : Goal.SoldGoodsToComplete)
	{
		if (Goal.SoldGoodsProgress.FindRef(RequiredGoods.Name) < RequiredGoods.Quantity) { return false; }
	}

	// Recipes crafted
	for (auto& RequiredRecipe : Goal.CraftedRecipesToComplete)
	{
		if (Goal.CraftedRecipesProgress.FindRef(RequiredRecipe.Key) < RequiredRecipe.Value) { return false; }
	}
	
	return true;
}


// Called every frame
void UActorGoalsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// Check goals list for newly available goals.
void UActorGoalsComponent::CheckForNewGoals()
{
	AFFGameMode * GameMode = (AFFGameMode *)GetWorld()->GetAuthGameMode();
	IGameplayGoalProvider * GoalProvider;
	if (GameMode) {
		GoalProvider = Cast<IGameplayGoalProvider>(GameMode);
		if (GoalProvider) {
			TArray<FGameplayGoal> NewGoals =  IGameplayGoalProvider::Execute_GetNewGameplayGoals(GameMode, CurrentGoals, CompletedGoals);
			CurrentGoals.Append(NewGoals);
			IGameplayGoalListener * OwnerListener = Cast<IGameplayGoalListener>(GetOwner());
			if (OwnerListener)
			{
				IGameplayGoalListener::Execute_OnNewGameplayGoals(GetOwner(), NewGoals);
			}
			//UE_LOG(LogTemp, Warning, TEXT("New Goals: %d"), NewGoals.Num());
		}
	}
}

// Check to see if any of our current goals qualify as completed.
void UActorGoalsComponent::CheckForCompletedGoals()
{
	TArray<FGameplayGoal> NewlyCompletedGoals;
	for (FGameplayGoal CurGoal : CurrentGoals)
	{
		if (CheckGoalQualifiesComplete(CurGoal)) { NewlyCompletedGoals.Add(CurGoal); }
	}

	for (FGameplayGoal CurGoal : NewlyCompletedGoals)
	{
		CurrentGoals.RemoveSingle(CurGoal);
		SetGoalComplete(CurGoal);
	}

	if (NewlyCompletedGoals.Num() > 0)
	{
		IGameplayGoalListener * OwnerListener = Cast<IGameplayGoalListener>(GetOwner());
		if (OwnerListener) { IGameplayGoalListener::Execute_OnGameplayGoalsCompleted(GetOwner(), NewlyCompletedGoals); }

		CheckForNewGoals();
	}
}

// Add a goal name to the list of completed goals.
void UActorGoalsComponent::SetGoalComplete(const FGameplayGoal & Goal)
{
	CompletedGoals.AddUnique(Goal.UniqueName);
}

void UActorGoalsComponent::UpdateHarvestedGoodsProgress(const TArray<FGoodsQuantity>& HarvestedGoods)
{
	if (HarvestedGoods.Num() == 0) { return; }
	bool bAnyUpdated = false;

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
					bAnyUpdated = true;
				}				
			}
		}
	}

	if (bAnyUpdated) { CheckForCompletedGoals(); }
}

void UActorGoalsComponent::UpdateSoldGoodsProgress(const TArray<FGoodsQuantity>& SoldGoods)
{
	if (SoldGoods.Num() == 0) { return; }
	bool bAnyUpdated = false;

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
					bAnyUpdated = true;
				}
			}
		}
	}

	if (bAnyUpdated) { CheckForCompletedGoals(); }
}

void UActorGoalsComponent::UpdateCraftedRecipesProgress(const FName & RecipeName, const int NumberCrafted)
{
	if (RecipeName.IsNone() || NumberCrafted == 0) { return; }
	bool bAnyUpdated = false;

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
				bAnyUpdated = true;
			}
		}
	}

	if (bAnyUpdated) { CheckForCompletedGoals(); }
}

