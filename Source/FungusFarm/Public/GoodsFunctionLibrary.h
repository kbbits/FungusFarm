// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GoodsQuantity.h"
#include "GoodsQuantityRange.h"
#include "RecipeQuantity.h"
#include "RecipeQuantityRange.h"
#include "GoodsDropChance.h"
#include "FungusFarm.h"
#include "GoodsFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class FUNGUSFARM_API UGoodsFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
		
public:

	// Transform a goods quantity range to a goods quantity. 
	// If optional QuantityScale is provided, the quantity will be mapped from min to max according to the scale (0-1) instead of determining randomly.
	UFUNCTION(BlueprintCallable, Category = "Goods")
		static FGoodsQuantity GoodsQuantityFromRange(const FGoodsQuantityRange& GoodsRange, const float QuantityScale = -1.0f /* 0.0 - 1.0 */);

	// Transform an array of goods quantity ranges to an array of goods quantities. 
	// If optional QuantityScale is provided, each quantity will be mapped from min to max according to the scale instead of determining randomly.
	UFUNCTION(BlueprintCallable, Category = "Goods")
		static TArray<FGoodsQuantity> GoodsQuantitiesFromRanges(const TArray<FGoodsQuantityRange>& GoodsRanges, const float QuantityScale = -1.0f /* 0.0 - 1.0 */);

	// Transform a recipe quantity range to a recipe quantity. 
	// If optional QuantityScale is provided, the quantity will be mapped from min to max according to the scale (0.0-1.0) instead of determining randomly.
	UFUNCTION(BlueprintCallable, Category = "Goods")
		static FRecipeQuantity RecipeQuantityFromRange(const FRecipeQuantityRange& RecipeRange, const float QuantityScale = -1.0f /* 0.0 - 1.0 */);

	// Transform an array of recipe quantity ranges to an array of recipe quantities. 
	// If optional QuantityScale is provided, each quantity will be mapped from min to max according to the scale instead of determining randomly.
	UFUNCTION(BlueprintCallable, Category = "Goods")
		static TArray<FRecipeQuantity> RecipeQuantitiesFromRanges(const TArray<FRecipeQuantityRange>& RecipeRanges, const float QuantityScale = -1.0f /* 0.0 - 1.0 */);

	UFUNCTION(BlueprintPure, Category = "Goods")
		static TArray<FGoodsQuantity> MultiplyGoodsQuantities(const TArray<FGoodsQuantity>& GoodsQuantities, const float Multiplier, const bool bTruncateQuantities = true);

	// Will not reduce quantities below 1. Rounds all quantities up to nearest integer.
	UFUNCTION(BlueprintPure, Category = "Goods")
		static TArray<FGoodsQuantity> ApplyGoodsQuantityFactor(const TArray<FGoodsQuantity>& GoodsQuantities, const float Multiplier);

	// Add two GoodsQuantity arrays together.
	// If bAllowNegativeSums == false, then sums that would result in a negative total (e.g. if one or more quantities being summed are negative), then the sums are not performed, 
	// bSuccessful will be returned false and the returned results will be an empty set.
	// If bIncludeZeros == true then all zero quantity entries will be included in results, if false they will be removed from results.
	UFUNCTION(BlueprintPure, Category = "Goods")
		static TArray<FGoodsQuantity> SumGoodsQuantities(const TArray<FGoodsQuantity>& GoodsOne, const TArray<FGoodsQuantity>& GoodsTwo, bool& bSuccessful, const bool bAllowNegativeTotals = false, const bool bIncludeZeros = false);

	// Subtracts quantities in GoodsToSubtract from GoodsOne.
	UFUNCTION(BlueprintPure, Category = "Goods")
		static TArray<FGoodsQuantity> SubtractGoodsQuantities(const TArray<FGoodsQuantity>& GoodsOne, const TArray<FGoodsQuantity>& GoodsToSubtract, bool& bSuccessful, const bool bAllowNegativeTotals = false);

	UFUNCTION(BlueprintPure, Category = "Goods")
		static TArray<FGoodsDropChance> MultiplyGoodsDropChanceQuantities(const TArray<FGoodsDropChance>& DropChances, const float Multiplier, const bool bTruncateQuantitiesToInteger = true);

	UFUNCTION(BlueprintPure, Category = "Goods")
		static FGoodsDropChance MultiplyGoodsDropChanceQuantity(const FGoodsDropChance& DropChance, const float Multiplier, const bool bTruncateQuantitiesToInteger = true);

	// Evaluates the GoodsDrop based on random chance.  Quantities evaluated are random in min-max range unless QuantityScale is > 0.0.
	UFUNCTION(BlueprintPure, Category = "Goods")
		static TArray<FGoodsQuantity> EvaluateGoodsDrop(const FGoodsDropChance& DropChance, const float QuantityScale = -1.0f /* 0.0 - 1.0 */);

	UFUNCTION(BlueprintPure, Category = "Goods")
		static TArray<FGoodsQuantity> EvaluateGoodsDrops(const TArray<FGoodsDropChance>& DropChances, const float QuantityScale = -1.0f /* 0.0 - 1.0 */);

	// Will take all ranges of goods and create a new array of GoodsDropChances with the min=max to a picked value so the DropChance now has a fixed quantity of goods.
	UFUNCTION(BlueprintPure, Category = "Goods")
		static TArray<FGoodsDropChance> FlattenToEvaluatedGoods(const TArray<FGoodsDropChance>& DropChances, const float QuantityScale = -1.0f /* 0.0 - 1.0 */);
		
	// Create a map from an array. Each element in the array must have a Name (FName) property.
	// New map has Name as key and element as value.  Collisions of Name in array are overwritten, last one wins.
	template<class T>
	static FORCEINLINE TMap<FName, T> NamedItemsToMap(TArray<T> NamedItems)
	{
		TMap<FName, T> NamedItemMap;
		for (T& NamedItem : NamedItems)
		{
			NamedItemMap.Add(NamedItem.Name, NamedItem);
		}
		return NamedItemMap;
	}

	// Create a map from an array. Each element in array must have a Name (FName) and a Quantity (int) property.
	// New map will have Name as key and Quantity as value. Collisions of Name in array are overwritten, last one wins.
	template<class T>
	static FORCEINLINE TMap<FName, int32> NamedQuantitiesToCountMap(TArray<T> NamedItems)
	{
		TMap<FName, int32> NamedCountMap;
		for (T& NamedItem : NamedItems)
		{
			NamedCountMap.Add(NamedItem.Name, static_cast<int32>(NamedItem.Quantity));
		}
		return NamedCountMap;
	}

	// For an array where each item has a WeightedChance (float) property that is > 0.0, this will pick one item from the array.
	// Item picked is a random selection in the weighted list. 
	// Can return nullptr if list is empty or results in negative total weight.
	template<class T>
	static FORCEINLINE T* PickOneFromWeightedList(TArray<T>& WeightedItems, const float TotalWeight = -1.0f)
	{
		float TotalWeightedChance = TotalWeight;
		float PickedWeight = 0.0f;
		float CumulativeWeight = 0.0;
		// Sum our total weights if it was not passed in.
		if (TotalWeightedChance < 0.0f)
		{
			TotalWeightedChance = 0.0f;
			for (T& WeightedItem : WeightedItems)
			{
				TotalWeightedChance += WeightedItem.WeightedChance;
			}
		}
		if (TotalWeightedChance <= 0.0f)
		{
			return nullptr;
		}

		PickedWeight = FMath::FRandRange(0.0, TotalWeightedChance);
		UE_LOG(LogFFGame, Warning, TEXT("PickOne total weight: %f  picked weight %f"), TotalWeightedChance, PickedWeight);

		// Iterate through our list of items until we find the first one where the overall PickedWeight is less than our cumulative total weight of items iterated so far.
		for (T& WeightedItem : WeightedItems)
		{
			CumulativeWeight += WeightedItem.WeightedChance;
			if (PickedWeight <= CumulativeWeight)
			{
				UE_LOG(LogFFGame, Warning, TEXT("Picked: %s"), *WeightedItem.UniqueNameBase.ToString())
				return &WeightedItem;
			}
		}
		return nullptr;
	}
};
