#pragma once

#include "Engine/DataTable.h"
#include "GoodsQuantity.h"
#include "GoodsDropChance.h"
#include "Guid.h"
#include "GameplayGoal.generated.h"

USTRUCT(BlueprintType)
struct FGameplayGoal : public FTableRowBase
{
	GENERATED_BODY()

public:
	bool operator== (const FGameplayGoal& other) const;
	bool operator== (const FGameplayGoal& other);
	bool operator== (FGameplayGoal& other);
	
	bool operator== (const FName& otherName) const;
	bool operator== (const FName& otherName);
	bool operator== (FName& otherName);
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		FName UniqueName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		FText DisplayName;

	// Message for user when goal becomes available for completion. (i.e. when "quest" is given to player)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		FText OnAvailableMessage;

	// Message to show when goal is completed.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		FText CompletedMessage;

	// Should this one be announced when it becomes available? If hidden it will not be announced.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		bool Hidden;

	// Cann the player abandon/ignore this goal?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		bool CanAbandon;

	// If <= 1, then the goal can only be completed once and will not become active again. If > 1, this is the maximum number of times 
	// the goal can be completed after which it will not longer become available. If = -1, it can repeat infinitely as long as goal provider exists.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		int32 MaxRepeats;

	// Has this goal been announced to the player?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		bool Announced;

	// Force the goal to be announced when available (if not hidden).
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		bool ForceAnnounce;

	// Goals that must be completed before this one is available.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TArray<FName> PrerequisiteGoals;

	// Recipes that must be unlocked before this goal becomes available.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TArray<FName> PrerequisiteRecipes;
	
	// Minimum player experience level for goal to become available. Set to 0 to ignore. Default = 0.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		int32 RequiredExperienceLevel;
	
	// Requirements for completing the goal
	
	// Goods harvested from "crops"
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TArray<FGoodsQuantity> HarvestedGoodsToComplete;

	// Goods sold to the Market
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TArray<FGoodsQuantity> SoldGoodsToComplete;

	// Goods that must be "given to the goal provider" in order to complete this goal
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TArray<FGoodsQuantity> DonatedGoodsToComplete;

	// Recipes that must be crafted
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TMap<FName, int32> CraftedRecipesToComplete;

	// If valid this image will be used in place of the goal provider's on incomplete goal screens
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TSoftObjectPtr<UTexture2D> OverrideImage;

	// If valid this image will be used in place of the goal provider's on the completed goal screen
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TSoftObjectPtr<UTexture2D> OverrideCompletedImage;

	// Goal requirements progress

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TMap<FName, float> HarvestedGoodsProgress;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TMap<FName, float> SoldGoodsProgress;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TMap<FName, float> DonatedGoodsProgress;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TMap<FName, int32> CraftedRecipesProgress;
	
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
		float ExperienceAwarded;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		FGuid ProviderGuid;
};

