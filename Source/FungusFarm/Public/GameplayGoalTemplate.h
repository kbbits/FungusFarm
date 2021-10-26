#pragma once

#include "Engine/DataTable.h"
#include "GoodsQuantityRange.h"
#include "GoodsDropChance.h"
#include "RecipeQuantityRange.h"
#include "Guid.h"
#include "GameplayGoalTemplate.generated.h"

USTRUCT(BlueprintType)
struct FGameplayGoalTemplate : public FTableRowBase
{
	GENERATED_BODY()

public:
	bool operator== (const FGameplayGoalTemplate& other) const;
	bool operator== (const FGameplayGoalTemplate& other);
	bool operator== (FGameplayGoalTemplate& other);

	bool operator== (const FName& otherName) const;
	bool operator== (const FName& otherName);
	bool operator== (FName& otherName);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		float WeightedChance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		FName UniqueNameBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		FText DisplayNameBase;

	// Message for user when goal becomes available for completion. (i.e. when "quest" is given to player)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		FText OnAvailableMessage;

	// Message to show when goal is completed.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		FText CompletedMessage;

	// Should this one be announced when it becomes available? If hidden it will not be announced.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		bool Hidden;

	// Can this goal be abandoned by the user?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		bool CanAbandon;

	// If <= 1, then the goal can only be completed once and will not become active again. If > 1, this is the maximum number of times 
	// the goal can be completed after which it will no longer become available. If = -1, it can repeat infinitely as long as goal provider exists.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		int32 MaxRepeats;

	// Goals that must be completed before this one is available.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TArray<FName> PrerequisiteGoals;

	// Recipes that must be unlocked before this goal becomes available.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TArray<FName> PrerequisiteRecipes;
	
	// Player level must be >= to this before this goal is available.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		float RequiredExperienceLevel;
	
	// Requirements for completing the goal
	
	// Goods harvested from "crops". This does not include spore "sprinkles".
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TArray<FGoodsQuantityRange> HarvestedGoodsToComplete;

	// Goods sold to the Market
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TArray<FGoodsQuantityRange> SoldGoodsToComplete;

	// Goods that must be "given to the goal provider" in order to complete this goal
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TArray<FGoodsQuantityRange> DonatedGoodsToComplete;

	// Recipes that must be crafted
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TArray<FRecipeQuantityRange> CraftedRecipesToComplete;

	// If valid this image will be used in place of the goal provider's on incomplete goal screens
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TSoftObjectPtr<UTexture2D> OverrideImage;

	// If valid this image will be used in place of the goal provider's on the completed goal screen
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TSoftObjectPtr<UTexture2D> OverrideCompletedImage;
		
	
	// Awards for completing the goal

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TArray<FName> UnlockedCraftingRecipes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TArray<FName> UnlockedCrops;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TArray<FName> UnlockedTools;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TArray<FName> OtherAwards;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TArray<FGoodsDropChance> GoodsAwarded;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		float ExperienceAwardedMinimum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		float ExperienceAwardedMaximum;
};

