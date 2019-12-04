// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SaveExtensionInterface.h"
#include "GameFramework/GameMode.h"
#include "SaveProfile.h"
#include "GameplayGoal.h"
#include "GameplayGoalProvider.h"
#include "GoalsProviderComponent.h"
#include "FFGameMode.generated.h"

/**
 * 
 */
UCLASS()
class FUNGUSFARM_API AFFGameMode : public AGameMode, public ISaveExtensionInterface
{
	GENERATED_BODY()

public:
	AFFGameMode(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, Category = "Components")
		UGoalsProviderComponent* GameplayGoalsProvider = nullptr;

	// For serialization.
	UPROPERTY(BlueprintReadOnly, SaveGame, Category = "Gameplay Goals")
		TArray<FName> SecondaryProviderNames;

	// Prefix for provider properties data files.  Will look in /Game/FungusFarm/Data/. Default= "GoalProviderProperties"
	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, Category = "Gameplay Goals")
		FString ProviderPropertiesTableName;

	// Prefix for provider goals data files.  Will look in /Game/FungusFarm/Data/. Default= "SecondaryGoals"
	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, Category = "Gameplay Goals")
		FString ProviderGoalsTablePrefix;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, Category = "Save Profile")
		int32 SaveSlotId;

protected:

	USaveProfile* CurrentSaveProfile;

	virtual void BeginPlay() override;

public:
	
	UFUNCTION(BlueprintCallable, Category = "Gameplay Goals")
		UGoalsProviderComponent* GetGoalProviderComponentByUniqueName(const FName ProviderUniqueName);

	UFUNCTION(BlueprintCallable, Category = "Gameplay Goals")
		UGoalsProviderComponent* AddSecondaryGoalProvider(const FName ProviderUniqueName);
	
	UFUNCTION(BlueprintCallable, Category = "Gameplay Goals")
		bool RemoveSecondaryGoalProvider(const FName ProviderUniqueName);

	UFUNCTION(BlueprintCallable, Category = "Gameplay Goals")
		TArray<UGoalsProviderComponent*> GetAllGoalProviders();

	UFUNCTION(BlueprintCallable, Category = "Gameplay Goals")
		TArray<UGoalsProviderComponent*> GetAllSecondaryGoalProviders();

	UFUNCTION(BlueprintCallable, Category = "Experience and Level")
		float GetExperienceRequiredForLevel(const int32 Level);

	// Save the whole game to a profile in the current SaveSlotId.  
	// This saves profile data and also calls the SaveExtension plugin to save actor data.
	UFUNCTION(BlueprintCallable, Category = "Save Profile")
		bool SaveGameProfile(const FString OverrideProfileName = "");

	// Loads the whole game from the current SaveSlotId.  
	// This loads profile data and also calls the SaveExtension plugin to load actors and data.
	UFUNCTION(BlueprintCallable, Category = "Save Profile")
		bool LoadGameProfile();

	// Gets the currently loaded SaveProfile or nullptr if no profile has been loaded.  
	// This does not load game data or change the profile state
	UFUNCTION(BlueprintCallable, Category = "Save Profile")
		FORCEINLINE USaveProfile * GetCurrentGameProfile() { return CurrentSaveProfile; }

	// Gets SaveProfile for the given SaveSlotId.  
	// This does not load game data or change the profile state
	UFUNCTION(BlueprintCallable, Category = "Save Profile")
		USaveProfile * GetGameProfile(const int32 SlotId, bool& bExists);

	// SaveExtension plugin notifcations
	// SaveExtension plugin Save Began
	virtual void OnSaveBegan() override;
	// SaveExtension plugin Load Began
	virtual void OnLoadBegan() override;
	// SaveExtension plugin Load Finished
	virtual void OnLoadFinished(bool bError) override;

private:

	FString GetSaveProfileFilename(const int32 SlotId);

};
