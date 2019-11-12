// Fill out your copyright notice in the Description page of Project Settings.


#include "FFGameMode.h"
#include "GameInstanceSubsystem.h"
#include "SaveManager.h"
#include "FFSlotInfo.h"
#include "FFPlayerState.h"
#include "GameplayGoalProviderTemplate.h"


AFFGameMode::AFFGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PlayerStateClass = AFFPlayerState::StaticClass();
	ProviderPropertiesTableName = FString("GoalProviderProperties");
	ProviderGoalsTablePrefix = FString("SecondaryGoals");

	GameplayGoalsProvider = ObjectInitializer.CreateDefaultSubobject<UGoalsProviderComponent>(this, TEXT("GoalsProvider"));
	UE_LOG(LogFFGame, Verbose, TEXT("FFGameMode constructed Goals Provider %s"), (GameplayGoalsProvider == nullptr ? TEXT("INVALID") : TEXT("VALID")));
}


void AFFGameMode::BeginPlay()
{
	Super::BeginPlay();	
	//UGameInstance* GInstance = GetGameInstance();
	USaveManager* SaveManager = USaveManager::GetSaveManager(this); //GInstance->GetSubsystem<USaveManager>();
	SaveManager->SubscribeForEvents(this);
}


UGoalsProviderComponent * AFFGameMode::GetGoalProviderComponentByUniqueName(const FName ProviderUniqueName)
{
	TArray<UGoalsProviderComponent*> AllProviders = GetAllGoalProviders();
	for (UGoalsProviderComponent* Provider : AllProviders)
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
				// Create a new component
				UGoalsProviderComponent* NewProvider = NewObject<UGoalsProviderComponent>(this, ProviderUniqueName);
				if (NewProvider)
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


TArray<UGoalsProviderComponent*> AFFGameMode::GetAllGoalProviders()
{
	TArray<UGoalsProviderComponent*> AllProviderComps;
	GetComponents<UGoalsProviderComponent>(AllProviderComps);
	return AllProviderComps;
}


TArray<UGoalsProviderComponent*> AFFGameMode::GetAllSecondaryGoalProviders()
{
	TArray<UGoalsProviderComponent*> AllProviderComps = GetAllGoalProviders();
	TArray<UGoalsProviderComponent*> SecondaryComps;
	for (UGoalsProviderComponent* CurComp : AllProviderComps)
	{
		if (CurComp != GameplayGoalsProvider)
		{
			SecondaryComps.Add(CurComp);
		}
	}
	return SecondaryComps;
}


float AFFGameMode::GetExperienceRequiredForLevel(const int32 Level)
{
	// TODO: implement a data-based approach for this.  For now just using a set scale.
	float CurLevel = static_cast<float>(Level);
	return (FMath::Pow(CurLevel, 1.2f) * 100.0f);
}


void AFFGameMode::OnSaveBegan()
{	
	USaveManager* SaveManager = USaveManager::GetSaveManager(this);
	USlotInfo* BaseSlotInfo = SaveManager->GetCurrentInfo();
	UE_LOG(LogFFGame, Warning, TEXT("Base info %s"), *FString(BaseSlotInfo == nullptr ? "INVALD" : "VALID"));
	UFFSlotInfo* SlotInfo = Cast<UFFSlotInfo>(BaseSlotInfo);
	TArray<UGoalsProviderComponent*> AllSecondaries = GetAllSecondaryGoalProviders();

	UE_LOG(LogFFGame, Log, TEXT("Save Began. Secondary providers %d"), AllSecondaries.Num());

	if (SlotInfo)
	{
		SlotInfo->SecondaryGoalProviders.Empty(AllSecondaries.Num());
		for (UGoalsProviderComponent* GoalComp : AllSecondaries)
		{
			SlotInfo->SecondaryGoalProviders.Add(GoalComp->UniqueName);
			//SecondaryProviderNames.Add(GoalComp->UniqueName);
		}
	}
	else
	{
		UE_LOG(LogFFGame, Warning, TEXT("Save Began. Slot id %d was not a valid FFSlotInfo."), SaveSlotId);
	}
}


void AFFGameMode::OnLoadBegan()
{
	USaveManager* SaveManager = USaveManager::GetSaveManager(this);
	UFFSlotInfo* SlotInfo = Cast<UFFSlotInfo>(SaveManager->GetSlotInfo(SaveSlotId));
	//UFFSlotInfo* SlotInfo = Cast<UFFSlotInfo>(SaveManager->GetCurrentInfo());
	if (SlotInfo)
	{
		UE_LOG(LogFFGame, Log, TEXT("Load Began. Creating secondary providers %d"), SlotInfo->SecondaryGoalProviders.Num());
		for (FName UniqueName : SlotInfo->SecondaryGoalProviders)
		{
			AddSecondaryGoalProvider(UniqueName);
		}
	}
	else
	{
		UE_LOG(LogFFGame, Warning, TEXT("Load Began. Slot id %d was not a valid FFSlotInfo."), SaveSlotId);
	}
}


void AFFGameMode::OnLoadFinished(bool bError)
{
	
}
