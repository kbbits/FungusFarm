// Fill out your copyright notice in the Description page of Project Settings.


#include "FFGameMode.h"
#include "GameInstanceSubsystem.h"
#include "SaveManager.h"
#include "FFPlayerState.h"
#include "GameplayGoalProviderTemplate.h"


AFFGameMode::AFFGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PlayerStateClass = AFFPlayerState::StaticClass();
	ProviderPropertiesTableName = FString("GoalProviderProperties");
	ProviderGoalsTablePrefix = FString("SecondaryGoals");

	GameplayGoalsProvider = ObjectInitializer.CreateDefaultSubobject<UGoalsProviderComponent>(this, TEXT("GoalsProvider"));
	AddOwnedComponent(GameplayGoalsProvider);
	//GameplayGoalsProvider->RegisterComponent();
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


UGoalsProviderComponent * AFFGameMode::GetGoalProviderForActiveGoal(const FGameplayGoal Goal)
{
	if (!Goal.ProviderGuid.IsValid())
	{
		UE_LOG(LogFFGame, Warning, TEXT("FFGameMode GetGoalProviderForGoal cannot find provider for invalid goal GUID."));
		return nullptr;
	}
	TArray<UGoalsProviderComponent*> AllProviders =  GetAllGoalProviders();
	for (UGoalsProviderComponent* Provider : AllProviders)
	{
		if (Provider->GetGoalProviderGuid() == Goal.ProviderGuid)
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
				NewProvider->ProviderImage = ProviderProperties->ProviderImage;
				NewProvider->GoalsData = ProviderGoalsTable;
				AddOwnedComponent(NewProvider);
				NewProvider->RegisterComponent();
				UE_LOG(LogFFGame, Log, TEXT("New secondary provider component added: %s"), *ProviderUniqueName.ToString());
				// Return the new provider
				return NewProvider;
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
	float CurLevel = static_cast<float>(Level) - 1;
	if (CurLevel <= 0) { return 0; }
	// https://www.desmos.com/calculator/luj0qz9dn6
	return FMath::TruncToFloat(FMath::Pow((CurLevel + FMath::FloorToInt(Level / 5.0f)), 2.0f) * 70.0f) * 10.0f;
}


bool AFFGameMode::SaveGameProfile(const FString ProfileName)
{
	UE_LOG(LogFFGame, Log, TEXT("FFGameMode SaveGameProfile: slot id %d"), SaveSlotId);
	if (SaveSlotId < 0) 
	{ 
		UE_LOG(LogFFGame, Warning, TEXT("FFGameMode SaveGameProfile: slot id cannot be < 0."));
		return false; 
	}
	FString ValidatedProfileName = FString(ProfileName);
	AFFPlayerState * PState = GetWorld()->GetFirstPlayerController()->GetPlayerState<AFFPlayerState>();
	TArray<UGoalsProviderComponent*> AllSecondaries = GetAllSecondaryGoalProviders();
	
	// First save the profile data
	if (!CurrentSaveProfile || !CurrentSaveProfile->IsValidLowLevel())
	{
		UE_LOG(LogFFGame, Log, TEXT("    Creating new save profile"));
		CurrentSaveProfile = NewObject<USaveProfile>(this, USaveProfile::StaticClass());
		if (ValidatedProfileName.Len() == 0)
		{
			ValidatedProfileName = FString::Printf(TEXT("Profile %d"), SaveSlotId);
		}
	}
	if (CurrentSaveProfile)
	{
		if (ValidatedProfileName.Len() > 0) 
		{ 
			CurrentSaveProfile->ProfileName = ProfileName; 
		}
		CurrentSaveProfile->SaveSlotId = SaveSlotId;
		CurrentSaveProfile->PlayerProperties = PState->PlayerProperties;
		CurrentSaveProfile->SecondaryGoalProviders.Empty(AllSecondaries.Num());
		for (UGoalsProviderComponent* GoalComp : AllSecondaries)
		{
			CurrentSaveProfile->SecondaryGoalProviders.Add(GoalComp->UniqueName);
		}
		if (!UGameplayStatics::SaveGameToSlot(CurrentSaveProfile, GetSaveProfileFilename(SaveSlotId), 0))
		{
			UE_LOG(LogFFGame, Warning, TEXT("FFGameMode SaveGameProfile: Error saving profile data."));
			return false;
		}
	}
	else
	{
		UE_LOG(LogFFGame, Warning, TEXT("FFGameMode SaveGameProfile: current save profile is null."));
		return false;
	}
	
	//UE_LOG(LogFFGame, Log, TEXT("FFGameMode SaveGameProfile: saved profile data."));
	
	// Call plugin to save rest of data
	UGameInstance* GInstance = GetGameInstance();
	USaveManager* SaveManager = GInstance->GetSubsystem<USaveManager>();
	if (!SaveManager) 
	{ 
		UE_LOG(LogFFGame, Warning, TEXT("FFGameMode SaveGameProfile: NO Save Manager")); 
		return false;
	}
	if (!SaveManager->SaveSlot(SaveSlotId, true, true, FScreenshotSize(640, 360)))
	{
		UE_LOG(LogFFGame, Warning, TEXT("FFGameMode SaveGameProfile: Error saving actor data."));
		return false;
	}

	//UE_LOG(LogFFGame, Log, TEXT("Profile %s saved."), *CurrentSaveProfile->ProfileName);
	
	return true;
}


bool AFFGameMode::LoadGameProfile()
{
	check(GetWorld());
	check(GetGameInstance());
	AFFPlayerState * PState = GetWorld()->GetFirstPlayerController()->GetPlayerState<AFFPlayerState>();
	
	// Load profile data	
	if (!UGameplayStatics::DoesSaveGameExist(GetSaveProfileFilename(SaveSlotId), 0))
	{
		return false;
	}
	CurrentSaveProfile = Cast<USaveProfile>(UGameplayStatics::LoadGameFromSlot(GetSaveProfileFilename(SaveSlotId), 0));
	if (!CurrentSaveProfile)
	{
		UE_LOG(LogFFGame, Warning, TEXT("FFGameMode LoadGameProfile: Error loading profile data."));
		return false;
	}
	PState->PlayerProperties = CurrentSaveProfile->PlayerProperties;
	for (FName UniqueName : CurrentSaveProfile->SecondaryGoalProviders)
	{	
		// Add the goal providers so they exist when plugin loads their data.
		AddSecondaryGoalProvider(UniqueName);
	}

	// Load actors, etc. with plugin
	UGameInstance* GInstance = GetGameInstance();
	USaveManager* SaveManager = GInstance->GetSubsystem<USaveManager>();
	if (!SaveManager)
	{
		UE_LOG(LogFFGame, Warning, TEXT("FFGameMode LoadGameProfile: NO Save Manager"));
		return false;
	}
	if (!SaveManager->LoadSlot(SaveSlotId))
	{
		UE_LOG(LogFFGame, Warning, TEXT("FFGameMode LoadGameProfile: Error loading actor data."));
		return false;
	}

	UE_LOG(LogFFGame, Log, TEXT("Profile %s loaded."), *CurrentSaveProfile->ProfileName);

	return true;
}


USaveProfile * AFFGameMode::GetGameProfile(const int32 SlotId, bool& bExists)
{
	USaveProfile * Profile = nullptr;
	// Load profile data	
	if (UGameplayStatics::DoesSaveGameExist(GetSaveProfileFilename(SlotId), 0))
	{
		USaveGame * SaveGame = UGameplayStatics::LoadGameFromSlot(GetSaveProfileFilename(SlotId), 0);
		if (!SaveGame) 
		{ 
			UE_LOG(LogFFGame, Warning, TEXT("FFGameMode GetGameProfile: Error loading game slot data.")); 
			bExists = false;
			return nullptr;
		}
		Profile = Cast<USaveProfile>(SaveGame);
		if (!Profile)
		{
			UE_LOG(LogFFGame, Warning, TEXT("FFGameMode GetGameProfile: Error loading profile data."));
			bExists = false;
			return nullptr;
		}
	}
	else
	{
		UE_LOG(LogFFGame, Warning, TEXT("FFGameMode GetGameProfile: data for profile %s does not exist."), *GetSaveProfileFilename(SlotId));
		bExists = false;
		return nullptr;
	}
	bExists = true;
	return Profile;
}


void AFFGameMode::OnSaveBegan()
{	
	/*
	TArray<UGoalsProviderComponent*> AllSecondaries = GetAllSecondaryGoalProviders();
	USaveManager* SaveManager = USaveManager::GetSaveManager(this);

	UE_LOG(LogFFGame, Log, TEXT("FFGameMode Save Began. Secondary providers %d"), AllSecondaries.Num());
	if (!SaveManager) { UE_LOG(LogFFGame, Log, TEXT("FFGameMode: NO Save Manager")); }
	
	USlotInfo* BaseSlotInfo = SaveManager->GetCurrentInfo();
	UFFSlotInfo* SlotInfo = Cast<UFFSlotInfo>(BaseSlotInfo);

	if (SlotInfo)
	{
		UE_LOG(LogFFGame, Log, TEXT("  FFGameMode: SlotInfo ID %d found. Saving FF data."), SlotInfo->Id);
		SlotInfo->SecondaryGoalProviders.Empty(AllSecondaries.Num());
		for (UGoalsProviderComponent* GoalComp : AllSecondaries)
		{
			SlotInfo->SecondaryGoalProviders.Add(GoalComp->UniqueName);
			//SecondaryProviderNames.Add(GoalComp->UniqueName);
		}
	}
	else
	{
		UE_LOG(LogFFGame, Warning, TEXT("FFGameMode Save Began. Slot id %d was not a valid FFSlotInfo."), SaveSlotId);
	}
	*/
}


void AFFGameMode::OnLoadBegan()
{
	/*
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
	*/
}


void AFFGameMode::OnLoadFinished(bool bError)
{
	
}


FString AFFGameMode::GetSaveProfileFilename(const int32 SlotId)
{
	return FString::Printf(TEXT("%d_profile"), SlotId);
}
