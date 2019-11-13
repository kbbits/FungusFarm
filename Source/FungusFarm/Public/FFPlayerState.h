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

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, SaveGame, Category = "FFPlayerState")
		FPlayerStateProperties PlayerProperties;	

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

protected:

	// Checks next level XP requirements and increments experience level by one if requirement is met.
	// Also triggers OnExperienceLevelChanged notifications if new level is different.
	UFUNCTION(BlueprintCallable, Category = "Experience and Level")
		bool CheckForLevelUp();
};
