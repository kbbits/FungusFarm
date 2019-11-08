#pragma once

#include "Engine/DataTable.h"
#include "GoodsQuantityRange.h"
#include "GoodsDropChance.h"
#include "Guid.h"
#include "GameplayGoalTemplate.generated.h"

USTRUCT(BlueprintType)
struct FGameplayGoalTemplate : public FTableRowBase
{
	GENERATED_BODY()

public:
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		bool CanAbandon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		bool CanRepeat;

	// Goals that must be completed before this one is available.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TArray<FName> PrerequisiteGoals;
	
	// Requirements for completing the goal
	
	// Goods harvested from "crops"
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
		TMap<FName, FGoodsQuantityRange> CraftedRecipesToComplete;
		
	
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
};

