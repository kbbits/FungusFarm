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
	// If optional QuantityScale is provided, the quantity will be mapped from min to max according to the scale (0-1) instead of determining randomly.
	UFUNCTION(BlueprintCallable, Category = "Goods")
		static FRecipeQuantity RecipeQuantityFromRange(const FRecipeQuantityRange& RecipeRange, const float QuantityScale = -1.0f /* 0.0 - 1.0 */);

	// Transform an array of recipe quantity ranges to an array of recipe quantities. 
	// If optional QuantityScale is provided, each quantity will be mapped from min to max according to the scale instead of determining randomly.
	UFUNCTION(BlueprintCallable, Category = "Goods")
		static TArray<FRecipeQuantity> RecipeQuantitiesFromRanges(const TArray<FRecipeQuantityRange>& RecipeRanges, const float QuantityScale = -1.0f /* 0.0 - 1.0 */);

	// Evaluates the GoodsDrop based on random chance.  Quantities evaluated are random in min-max range unless QuantityScale is 0.0 - 1.0.
	UFUNCTION(BlueprintCallable, Category = "Goods")
		static TArray<FGoodsQuantity> EvaluateGoodsDrop(const FGoodsDropChance& DropChance, const float QuantityScale = -1.0f /* 0.0 - 1.0 */);

	UFUNCTION(BlueprintCallable, Category = "Goods")
		static TArray<FGoodsQuantity> EvaluateGoodsDrops(const TArray<FGoodsDropChance>& DropChances, const float QuantityScale = -1.0f /* 0.0 - 1.0 */);

	// Will take all ranges of goods and set the min=max to a picked value so the DropChance now has a fixed range of goods.
	UFUNCTION(BlueprintCallable, Category = "Goods")
		static TArray<FGoodsDropChance> FlattenToEvaluatedGoods(const TArray<FGoodsDropChance>& DropChances, const float QuantityScale = -1.0f /* 0.0 - 1.0 */);
		
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

	//
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

	template<class T>
	static FORCEINLINE T* PickOneFromWeightedList(TArray<T>& WeightedItems, const float TotalWeight = -1.0f)
	{
		float TotalWeightedChance = TotalWeight;
		float PickedWeight = 0.0f;
		float CumulativeWeight = 0.0;
		if (TotalWeightedChance < 0.0f)
		{
			TotalWeightedChance = 0.0f;
			for (T& WeightedItem : WeightedItems)
			{
				TotalWeightedChance += WeightedItem.WeightedChance;
			}
		}
		PickedWeight = FMath::FRandRange(0.0, TotalWeightedChance);

		UE_LOG(LogFFGame, Warning, TEXT("PickOne total weight: %f  picked weight %f"), TotalWeightedChance, PickedWeight);

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
