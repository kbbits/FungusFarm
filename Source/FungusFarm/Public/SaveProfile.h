// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PlayerStateProperties.h"
#include "SaveProfile.generated.h"

/**
 * Used to store info for the Player's profile and current game.
 * Game actor data is saved by plugin (SaveExtension).
 */
UCLASS()
class FUNGUSFARM_API USaveProfile : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, Category = "Save Profile")
		int32 SaveSlotId;

	UPROPERTY(BlueprintReadWrite, Category = "Save Profile")
		FString ProfileName;

	UPROPERTY(BlueprintReadWrite, Category = "Save Profile")
		float PlayTime;
	
	UPROPERTY(BlueprintReadWrite, Category = "Save Profile")
		FPlayerStateProperties PlayerProperties;

	UPROPERTY(BlueprintReadOnly, Category = "Save Profile")
		TArray<FName> SecondaryGoalProviders;
	
};
