#pragma once

#include "Engine/DataTable.h"
#include "GoodsQuantity.h"
#include "GoodsDropChance.h"
#include "GameplayGoal.generated.h"

USTRUCT(BlueprintType)
struct FGameplayGoal : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		FText DisplayDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TArray<FName> PrerequisiteGoals;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TArray<FGoodsQuantity> TotalHarvestedGoodsToComplete;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TMap<FName, int32> CraftedRecipesToComplete;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TArray<FName> UnlockedCraftingRecipes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TArray<FName> UnlockedCrops;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TArray<FGoodsDropChance> GoodsAwarded;
};