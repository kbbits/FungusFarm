// Fill out your copyright notice in the Description page of Project Settings.


#include "FFGameMode.h"
#include "GameInstanceSubsystem.h"
#include "SaveManager.h"
#include "FFPlayerState.h"
#include "GameplayGoalProviderTemplate.h"


AFFGameMode::AFFGameMode()
	: Super()
{
	PlayerStateClass = AFFPlayerState::StaticClass();
	ProviderPropertiesTableName = FString("GoalProviderProperties");
	ProviderGoalsTablePrefix = FString("SecondaryGoals");
	UnpausedTimeAtLastSave = 0.0f;

	GameplayGoalsProvider = CreateDefaultSubobject<UGoalsProviderComponent>(TEXT("GoalsProvider"));
	if (GameplayGoalsProvider) {
		AddOwnedComponent(GameplayGoalsProvider);
	} else {
		UE_LOG(LogFFGame, Error, TEXT("FFGameMode errror constructing Goals Provider"));
	}
	UE_LOG(LogFFGame, Verbose, TEXT("FFGameMode constructed Goals Provider %s"), (GameplayGoalsProvider == nullptr ? TEXT("INVALID") : TEXT("VALID")));
}


FString AFFGameMode::GetSaveProfileFilename(const int32 SlotId)
{
	return FString::Printf(TEXT("%d_profile"), SlotId);
}


void AFFGameMode::BeginPlay()
{
	Super::BeginPlay();	
	//UGameInstance* GInstance = GetGameInstance();
	USaveManager* SaveManager = USaveManager::Get(this); //GInstance->GetSubsystem<USaveManager>();
	SaveManager->SubscribeForEvents(this);
}


float AFFGameMode::GetGlobalCropGrowthFactor()
{
	if (GameType.DifficultySettings.CropGrowthFactor > 0.0f) {
		return GameType.DifficultySettings.CropGrowthFactor;
	}
	return 1.0f;
}


float AFFGameMode::GetGlobalCropHarvestFactor()
{
	if (GameType.DifficultySettings.CropHarvestFactor > 0.0f) {
		return GameType.DifficultySettings.CropHarvestFactor;
	}
	return 1.0f;
}


float AFFGameMode::GetGlobalCraftingTimeFactor()
{
	if (GameType.DifficultySettings.CraftingTimeFactor > 0.0f) {
		return GameType.DifficultySettings.CraftingTimeFactor;
	}
	return 1.0f;
}


float AFFGameMode::GetGlobalCraftingIngredientsFactor()
{
	if (GameType.DifficultySettings.CraftingIngredientsFactor > 0.0f) {
		return GameType.DifficultySettings.CraftingIngredientsFactor;
	}
	return 1.0f;
}


float AFFGameMode::GetGlobalCraftingResultsFactor()
{
	if (GameType.DifficultySettings.CraftingResultsFactor > 0.0f) {
		return GameType.DifficultySettings.CraftingResultsFactor;
	}
	return 1.0f;
}


float AFFGameMode::GetGlobalExperienceRequiredFactor()
{
	if (GameType.DifficultySettings.ExperienceRequiredFactor > 0.0f) {
		return GameType.DifficultySettings.ExperienceRequiredFactor;
	}
	return 1.0f;
}


float AFFGameMode::GetGlobalExperienceRequiredExponent()
{
	if (GameType.DifficultySettings.ExperienceRequiredExponent > 0) {
		return GameType.DifficultySettings.ExperienceRequiredExponent;
	}
	return 2.2f;
}


bool AFFGameMode::CropClicksCountAsHarvest()
{
	return GameType.DifficultySettings.bCropClicksCountAsHarvest;
}


UDataTable * AFFGameMode::GetGoalProviderData(const FName ProviderUniqueName)
{
	FString GoalsDataPath = ("/Game/FungusFarm/Data/" + ProviderGoalsTablePrefix + "_" + ProviderUniqueName.ToString() + "." + ProviderGoalsTablePrefix + "_" + ProviderUniqueName.ToString());
	//UE_LOG(LogFFGame, Log, TEXT("looking for table: %s"), *GoalsDataPath);

	// Lookup provider goal data in data table and apply them to component
	UObject* DataObj = StaticLoadObject(UDataTable::StaticClass(), this, *GoalsDataPath);
	if (!DataObj) {
		UE_LOG(LogFFGame, Error, TEXT("%s GetGoalProviderData unknown goal data table: %s"), *GetNameSafe(this), *GoalsDataPath);
	}
	return DataObj ? Cast<UDataTable>(DataObj) : nullptr;
}


FGameplayGoal AFFGameMode::GetGoalData(const FName ProviderUniqueName, const FName GoalName)
{
	UDataTable* GoalsData = GetGoalProviderData(ProviderUniqueName);
	if (GoalsData) {
		return *GoalsData->FindRow<FGameplayGoal>(GoalName, TEXT("GetGoalData"), false);
	}
	else
	{
		UE_LOG(LogFFGame, Error, TEXT("%s GetGoalData goal data table not found for provider: %s"), *GetNameSafe(this), *ProviderUniqueName.ToString());
		return FGameplayGoal();
	}	
}


UGoalsProviderComponent * AFFGameMode::GetGoalProviderComponentByUniqueName(const FName ProviderUniqueName)
{
	TArray<UGoalsProviderComponent*> AllProviders = GetAllGoalProviders();
	for (UGoalsProviderComponent* Provider : AllProviders)
	{
		if (Provider->GetGameplayGoalProviderUniqueName() == ProviderUniqueName) {
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
		if (Provider->GetGoalProviderGuid() == Goal.ProviderGuid) {
			return Provider;
		}
	}
	return nullptr;
}


UGoalsProviderComponent* AFFGameMode::AddSecondaryGoalProvider(const FName ProviderUniqueName)
{
	// If we already have this provider, just return the existing one.
	UGoalsProviderComponent* ExistingProvider = GetGoalProviderComponentByUniqueName(ProviderUniqueName);
	if (ExistingProvider) {
		return ExistingProvider;
	}

	// Lookup provider properties in data table
	FString ProviderPropertiesPath("/Game/FungusFarm/Data/" + ProviderPropertiesTableName + "." + ProviderPropertiesTableName);
	UObject* PropertiesObj = StaticLoadObject(UDataTable::StaticClass(), this, *ProviderPropertiesPath);
	if (!PropertiesObj) {
		UE_LOG(LogFFGame, Warning, TEXT("%s AddSecondaryGoalProvider unknown goal provider properties data table: %s"), *GetNameSafe(this), *ProviderPropertiesPath);
	}
	UDataTable* ProviderPropertiesTable = PropertiesObj ? Cast<UDataTable>(PropertiesObj) : nullptr;
	UDataTable* ProviderGoalsTable = GetGoalProviderData(ProviderUniqueName);
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
		if (CurComp != GameplayGoalsProvider) {
			SecondaryComps.Add(CurComp);
		}
	}
	return SecondaryComps;
}


float AFFGameMode::GetExperienceRequiredForLevel(const int32 Level)
{
	// TODO: implement a data-based approach for this?  For now just using a formula.
	const float CurLevel = static_cast<float>(Level) - 1;
	if (CurLevel <= 0) { return 0; }
	// https://www.desmos.com/calculator/luj0qz9dn6
	float BaseRequired = (FMath::TruncToFloat((FMath::Pow(CurLevel + FMath::FloorToInt(Level / 5.0f), GetGlobalExperienceRequiredExponent()) * 10.0f) + (CurLevel * 5)) * 100.0f) + 1200.0f;
	// Levels above 10 require more xp
	if (Level > 10) {
		BaseRequired += FMath::TruncToFloat(FMath::Pow(Level - 10, 1 + (0.02 * (Level - 11)))) * 10000.0f;
	}
	return BaseRequired * GetGlobalExperienceRequiredFactor();
}	


float AFFGameMode::GetUnpausedTimeSinceLastSave()
{
	return UGameplayStatics::GetUnpausedTimeSeconds(this) - UnpausedTimeAtLastSave;
}


float AFFGameMode::GetTotalUnpausedTime()
{
	return PreviousTotalUnpausedTime + GetUnpausedTimeSinceLastSave();
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
	bool bExists;
	
	// First save the profile data
	// Find existing
	if (!CurrentSaveProfile || !CurrentSaveProfile->IsValidLowLevel())
	{
		CurrentSaveProfile = GetGameProfile(SaveSlotId, bExists);
		if (!bExists || !CurrentSaveProfile || !CurrentSaveProfile->IsValidLowLevel())
		{
			// Create a new profile
			UE_LOG(LogFFGame, Log, TEXT("    Creating new save profile"));
			CurrentSaveProfile = NewObject<USaveProfile>(this, USaveProfile::StaticClass());
			if (ValidatedProfileName.Len() == 0) {
				ValidatedProfileName = FString::Printf(TEXT("Profile %d"), SaveSlotId);
			}
		}
	}
	// Save the proflie data
	if (CurrentSaveProfile)
	{
		if (ValidatedProfileName.Len() > 0) { 
			CurrentSaveProfile->ProfileName = ProfileName; 
		}
		CurrentSaveProfile->SaveSlotId = SaveSlotId;
		CurrentSaveProfile->GameTypeName = GameType.Name;
		CurrentSaveProfile->GameTypeDisplayName = GameType.DisplayName;
		CurrentSaveProfile->PlayerProperties = PState->PlayerProperties;
		CurrentSaveProfile->PlayTime += GetUnpausedTimeSinceLastSave();
		CurrentSaveProfile->SecondaryGoalProviders.Empty(AllSecondaries.Num());
		for (UGoalsProviderComponent* GoalComp : AllSecondaries) {
			CurrentSaveProfile->SecondaryGoalProviders.Add(GoalComp->UniqueName);
		}
		if (!UGameplayStatics::SaveGameToSlot(CurrentSaveProfile, GetSaveProfileFilename(SaveSlotId), 0))
		{
			UE_LOG(LogFFGame, Warning, TEXT("FFGameMode SaveGameProfile: Error saving profile data."));
			return false;
		}
		UnpausedTimeAtLastSave = UGameplayStatics::GetUnpausedTimeSeconds(this);
		PreviousTotalUnpausedTime = CurrentSaveProfile->PlayTime;
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
		UE_LOG(LogFFGame, Warning, TEXT("FFGameMode LoadGameProfile: No profile found with id %d"), SaveSlotId);
		return false;
	}
	CurrentSaveProfile = Cast<USaveProfile>(UGameplayStatics::LoadGameFromSlot(GetSaveProfileFilename(SaveSlotId), 0));
	if (!CurrentSaveProfile)
	{
		UE_LOG(LogFFGame, Warning, TEXT("FFGameMode LoadGameProfile: Error loading profile data."));
		return false;
	}
	// Game type info does not need to be loaded. They are saved on the GameMode itself.
	PState->PlayerProperties = CurrentSaveProfile->PlayerProperties;
	for (FName UniqueName : CurrentSaveProfile->SecondaryGoalProviders)
	{	
		// Add the goal providers so they exist when plugin loads their data.
		AddSecondaryGoalProvider(UniqueName);
	}
	PreviousTotalUnpausedTime = CurrentSaveProfile->PlayTime;

	// Load game mode, actors, etc. with plugin
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
	UnpausedTimeAtLastSave = UGameplayStatics::GetUnpausedTimeSeconds(this);	

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


bool AFFGameMode::DeleteGameProfile(const int32 SlotId)
{
	bool bSuccess = false;
	// Load profile data	
	if (UGameplayStatics::DoesSaveGameExist(GetSaveProfileFilename(SlotId), 0))
	{
		bSuccess = UGameplayStatics::DeleteGameInSlot(GetSaveProfileFilename(SlotId), 0);
		UGameInstance* GInstance = GetGameInstance();
		USaveManager* SaveManager = GInstance->GetSubsystem<USaveManager>();
		if (SaveManager && bSuccess) {
			bSuccess = SaveManager->DeleteSlotById(SlotId);
		}
	}
	return bSuccess;
}


void AFFGameMode::OnSaveBegan(const FSELevelFilter& Filter)
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


void AFFGameMode::OnSaveFinished(const FSELevelFilter& Filter, bool bError)
{

}


void AFFGameMode::OnLoadBegan(const FSELevelFilter& Filter)
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


void AFFGameMode::OnLoadFinished(const FSELevelFilter& Filter, bool bError)
{
	
}

