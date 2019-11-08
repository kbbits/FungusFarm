// Fill out your copyright notice in the Description page of Project Settings.


#include "FFGameMode.h"


AFFGameMode::AFFGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GameplayGoalsProvider = ObjectInitializer.CreateDefaultSubobject<UGoalsProviderComponent>(this, TEXT("GoalsProvider"));
	UE_LOG(LogFFGame, VeryVerbose, TEXT("FFGameMode constructed Goals Provider %s"), (GameplayGoalsProvider == nullptr ? TEXT("INVALID") : TEXT("VALID")));
}

void AFFGameMode::BeginPlay()
{
	Super::BeginPlay();
		
}

UGoalsProviderComponent* AFFGameMode::AddSecondaryGoalProvider(const FName ProviderUniqueName)
{
	UGoalsProviderComponent* NewProvider = NewObject<UGoalsProviderComponent>(this, ProviderUniqueName);
	
	if (NewProvider)
	{
		FString GoalsDataPath = ("/Game/FungusFarm/Data/SecondaryGoals_" + ProviderUniqueName.ToString() + ".SecondaryGoals_" + ProviderUniqueName.ToString());
		//UE_LOG(LogFFGame, Warning, TEXT("looking for table: %s"), *GoalsDataPath);
		
		// Lookup provider properties in data table and apply them to component
		UObject* LoadedObj = StaticLoadObject(UDataTable::StaticClass(), this, *GoalsDataPath);
		UDataTable* ProviderTable = Cast<UDataTable>(LoadedObj);
		if (ProviderTable && ProviderTable->GetRowMap().Num() > 0)
		{			
			//UE_LOG(LogFFGame, Warning, TEXT("Secondary goals table struct: %s"),  *ProviderTable->GetRowStructName().ToString());
			FGameplayGoal* TmpGoal = ProviderTable->FindRow<FGameplayGoal>(ProviderTable->GetRowNames()[0], FString("GameMode AddSecondaryGoalProvider"));
			if (TmpGoal)
			{
				NewProvider->bAutoActivate = true;
				NewProvider->bDisableNewGoals = true;
				NewProvider->DelayBetweenNewGoals = 10.0;
				NewProvider->MaximumCurrentGoals = 1;
				NewProvider->UniqueName = ProviderUniqueName;
				NewProvider->FriendlyName = ProviderUniqueName.ToString();
				NewProvider->GoalsData = ProviderTable;
				AddOwnedComponent(NewProvider);

				NewProvider->RegisterComponent();
				UE_LOG(LogFFGame, Warning, TEXT("New secondary provider component added: %s"), *ProviderUniqueName.ToString());
				return NewProvider;
			}
		}		
	}
	return nullptr;
}

// Return true if goal provider component was removed.
bool AFFGameMode::RemoveSecondaryGoalProvider(const FName ProviderUniqueName)
{
	TArray<UGoalsProviderComponent*> AllSecondaries;
	GetComponents(AllSecondaries); 
	for (UGoalsProviderComponent* Provider : AllSecondaries)
	{
		if (Provider != GameplayGoalsProvider && Provider->GetGameplayGoalProviderUniqueName() == ProviderUniqueName)
		{
			Provider->DestroyComponent();
			return true;
		}
	}
	return false;
}

/*
void AFFGameMode::OnGameplayGoalComplete(const FGameplayGoal & CompletedGoal)
{
	if (!CompletedGoal.ProviderGuid.IsValid())
	{
		return;
	}

	TArray<UGoalsProviderComponent*> AllProviders;
	GetComponents(AllProviders);
	for (UGoalsProviderComponent* Provider : AllProviders)
	{
		if (Provider->GetGameplayGoalProviderGuid() == CompletedGoal.ProviderGuid)
		{
			Provider->OnGameplayGoalCompleted(CompletedGoal);
		}
	}
}
*/


