// Fill out your copyright notice in the Description page of Project Settings.


#include "FFGameMode.h"
#include "GameplayGoalProviderTemplate.h"


AFFGameMode::AFFGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ProviderPropertiesTableName = FString("GoalProviderProperties");
	ProviderGoalsTablePrefix = FString("SecondaryGoals");

	GameplayGoalsProvider = ObjectInitializer.CreateDefaultSubobject<UGoalsProviderComponent>(this, TEXT("GoalsProvider"));
	UE_LOG(LogFFGame, Verbose, TEXT("FFGameMode constructed Goals Provider %s"), (GameplayGoalsProvider == nullptr ? TEXT("INVALID") : TEXT("VALID")));
}


void AFFGameMode::BeginPlay()
{
	Super::BeginPlay();	
}


UGoalsProviderComponent * AFFGameMode::GetGoalProviderComponentByUniqueName(const FName ProviderUniqueName)
{
	TArray<UGoalsProviderComponent*> AllSecondaries;
	GetComponents(AllSecondaries);
	for (UGoalsProviderComponent* Provider : AllSecondaries)
	{
		if (Provider->GetGameplayGoalProviderUniqueName() == ProviderUniqueName)
		{
			return Provider;
		}
	}
	return nullptr;
}


UGoalsProviderComponent* AFFGameMode::AddSecondaryGoalProvider(const FName ProviderUniqueName)
{
	// If we already have this provider, just return the existing one.
	UGoalsProviderComponent* ExistingProvider = GetGoalProviderComponentByUniqueName(ProviderUniqueName);
	if (ExistingProvider)
	{
		return ExistingProvider;
	}

	// Create a new component
	UGoalsProviderComponent* NewProvider = NewObject<UGoalsProviderComponent>(this, ProviderUniqueName);
	if (NewProvider)
	{
		FString ProviderPropertiesPath("/Game/FungusFarm/Data/" + ProviderPropertiesTableName + "." + ProviderPropertiesTableName);
		FString GoalsDataPath = ("/Game/FungusFarm/Data/" + ProviderGoalsTablePrefix + "_" + ProviderUniqueName.ToString() + "." + ProviderGoalsTablePrefix + "_" + ProviderUniqueName.ToString());
		//UE_LOG(LogFFGame, Log, TEXT("looking for table: %s"), *GoalsDataPath);

		// Lookup provider properties in data table
		UObject* PropertiesObj = StaticLoadObject(UDataTable::StaticClass(), this, *ProviderPropertiesPath);
		if (!PropertiesObj)
		{
			UE_LOG(LogFFGame, Warning, TEXT("%s AddSecondaryGoalProvider unknown goal provider properties data table: %s"), *GetNameSafe(this), *ProviderPropertiesPath);
		}
		// Lookup provider goal data in data table and apply them to component
		UObject* DataObj = StaticLoadObject(UDataTable::StaticClass(), this, *GoalsDataPath);
		if (!DataObj)
		{
			UE_LOG(LogFFGame, Warning, TEXT("%s AddSecondaryGoalProvider unknown goal data table: %s"), *GetNameSafe(this), *GoalsDataPath);
		}
		UDataTable* ProviderPropertiesTable = PropertiesObj ? Cast<UDataTable>(PropertiesObj) : nullptr;
		UDataTable* ProviderGoalsTable = DataObj ? Cast<UDataTable>(DataObj) : nullptr; 
		// Find our properties in the table
		FGameplayGoalProviderTemplate* ProviderProperties = ProviderPropertiesTable->FindRow<FGameplayGoalProviderTemplate>(ProviderUniqueName, FString("GameMode AddSecondaryGoalProvider"));
		if (ProviderPropertiesTable && ProviderProperties)
		{
			if (ProviderGoalsTable && ProviderGoalsTable->GetRowMap().Num() > 0)
			{
				//UE_LOG(LogFFGame, Log, TEXT("Secondary goals table struct: %s"),  *ProviderTable->GetRowStructName().ToString());
				// Get a TmpGoal record to make sure they are the correct type.
				FGameplayGoal* TmpGoal = ProviderGoalsTable->FindRow<FGameplayGoal>(ProviderGoalsTable->GetRowNames()[0], FString("GameMode AddSecondaryGoalProvider"));
				if (TmpGoal)
				{
					NewProvider->bAutoActivate = true;
					NewProvider->bDisableNewGoals = true;
					NewProvider->DelayBetweenNewGoalsMin = ProviderProperties->DelayBetweenNewGoalsMinimum;
					NewProvider->DelayBetweenNewGoalsMax = ProviderProperties->DelayBetweenNewGoalsMaximum;
					NewProvider->MaximumCurrentGoals = ProviderProperties->MaximumCurrentGoals;
					NewProvider->UniqueName = ProviderUniqueName;
					NewProvider->FriendlyName = ProviderProperties->FriendlyName.ToString();
					NewProvider->GoalsData = ProviderGoalsTable;
					AddOwnedComponent(NewProvider);
					NewProvider->RegisterComponent();
					UE_LOG(LogFFGame, Log, TEXT("New secondary provider component added: %s"), *ProviderUniqueName.ToString());
					// Return the new provider
					return NewProvider;
				}
				else
				{
					UE_LOG(LogFFGame, Warning, TEXT("%s AddSecondaryGoalProvider goals data table is empty or rows are not FGameplayGoal: %s"), *GetNameSafe(this), *GoalsDataPath);
				}
			}
		}
		else
		{
			UE_LOG(LogFFGame, Warning, TEXT("%s AddSecondaryGoalProvider no row with name %s in provider properties table or rows are not FGameplayGoalProviderTempate"), *GetNameSafe(this), *ProviderUniqueName.ToString());
		}
	}
	return nullptr;
}

// Removes the component from the game mode.
// Return true if goal provider component was removed.
bool AFFGameMode::RemoveSecondaryGoalProvider(const FName ProviderUniqueName)
{
	UGoalsProviderComponent* Provider = GetGoalProviderComponentByUniqueName(ProviderUniqueName);
	if (Provider)
	{
		// Don't allow removing our defalt component
		if (Provider != GameplayGoalsProvider && Provider->GetGameplayGoalProviderUniqueName() == ProviderUniqueName)
		{
			Provider->DestroyComponent();
			UE_LOG(LogFFGame, Log, TEXT("%s destroyed secondary provider component: %s"), *GetNameSafe(this), *ProviderUniqueName.ToString());
			return true;
		}
	}
	return false;
}


