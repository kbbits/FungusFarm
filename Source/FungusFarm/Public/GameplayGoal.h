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

	// Has this goal been announced to the player?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		bool Announced;

	// Goals that must be completed before this one is available.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TArray<FName> PrerequisiteGoals;

	
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
		FGuid ProviderGuid;
};

