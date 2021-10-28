// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Delegates/Delegate.h"
#include "PlayerStateProperties.h"
#include "FFPlayerState.generated.h"

// Event dispatcher for when experience points has changed
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnExperienceChanged, const float, NewExperience);
// Event dispatcher notifying experience level has changed
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnExperienceLevelChanged, const int32, NewExperienceLevel);
// Event dispatcher notifying recipe has been unlocked
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRecipeUnlocked, const FName, RecipeName);
// Event dispatcher notifying crop has been unlocked
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCropUnlocked, const FName, CropName);
// Event dispatcher notifying tool has been unlocked
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnToolUnlocked, const FName, ToolName);

/**
 * 
 */
UCLASS()
class FUNGUSFARM_API AFFPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AFFPlayerState(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
		FOnExperienceChanged OnExperienceChanged;

	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
		FOnExperienceLevelChanged OnExperienceLevelChanged;

	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
		FOnRecipeUnlocked OnRecipeUnlocked;

	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
		FOnCropUnlocked OnCropUnlocked;

	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
		FOnToolUnlocked OnToolUnlocked;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "FFPlayerState")
		FPlayerStateProperties PlayerProperties;	

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, SaveGame, Category = "FFPlayerState")
		TArray<FName> UnlockedRecipes;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, SaveGame, Category = "FFPlayerState")
		TArray<FName> UnlockedCrops;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, SaveGame, Category = "FFPlayerState")
		TArray<FName> UnlockedTools;

public:

	UFUNCTION(BlueprintPure, Category = "Experience and Level")
		float GetExperience();

	// Add a given amount of experience. Also triggers OnExperienceChanged notifications.
	UFUNCTION(BlueprintCallable, Category = "Experience and Level")
		float AddExperience(const float Experience);

	// Set experience points to a specific value. Also triggers OnExperienceChanged notifications if new experience is different.
	UFUNCTION(BlueprintCallable, Category = "Experience and Level")
		void SetExperience(const float Experience);

	UFUNCTION(BlueprintPure, Category = "Experience and Level")
		int32 GetExperienceLevel();

	// Sets experience level to a specific value. Also triggers OnExperienceLevelChanged notifications if new level is different.
	UFUNCTION(BlueprintCallable, Category = "Experience and Level")
		void SetExperienceLevel(const int32 NewLevel);

	// Returns the amount of experience required for the player's next level. (current level + 1)
	UFUNCTION(BlueprintPure, Category = "Experience and Level")
		float GetExperienceForNextLevel();

	// Sets the index for the last used navigation target associated with the given level.
	UFUNCTION(BlueprintCallable, Category = "FFPlayerState")
		void SetNavIndexForLevel(FName LevelName, int32 Index);

	// Gets the index for the last used navigation target associated with the given level.
	UFUNCTION(BlueprintPure, Category = "FFPlayerState")
		int32 GetNavIndexForLevel(FName LevelName);

	UFUNCTION(BlueprintPure, Category = "Recipes")
		bool IsRecipeUnlocked(FName RecipeName);

	UFUNCTION(BlueprintPure, Category = "Recipes")
		bool AreRecipesUnlocked(TArray<FName> RecipeNames);

	UFUNCTION(BlueprintCallable, Category = "Recipes")
		void UnlockRecipe(FName RecipeName);

	UFUNCTION(BlueprintCallable, Category = "Crops")
		bool IsCropUnlocked(FName CropName);

	UFUNCTION(BlueprintCallable, Category = "Crops")
		void UnlockCrop(FName CropName);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Tools")
		void UnlockTool(const FName ToolName);

	UFUNCTION(BlueprintCallable, Category = "Tools")
		bool IsToolUnlocked(const FName ToolName);

protected:

	// Checks next level XP requirements and increments experience level by one if requirement is met.
	// Also triggers OnExperienceLevelChanged notifications if new level is different.
	UFUNCTION(BlueprintCallable, Category = "Experience and Level")
		bool CheckForLevelUp();
};
