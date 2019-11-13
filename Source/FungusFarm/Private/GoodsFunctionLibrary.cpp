// Fill out your copyright notice in the Description page of Project Settings.


#include "GoodsFunctionLibrary.h"

FGoodsQuantity UGoodsFunctionLibrary::GoodsQuantityFromRange(const FGoodsQuantityRange& GoodsRange, const float QuantityScale /* 0.0 - 1.0 */)
{
	if (GoodsRange.QuantityMin == GoodsRange.QuantityMax)
	{
		return FGoodsQuantity(GoodsRange.Name, GoodsRange.QuantityMax);
	}
	if (QuantityScale >= 0.0f)
	{
		// Range is in min-max range determined by scale
		float ClampedScale = FMath::Max<float>(QuantityScale, 1.0f);
		return FGoodsQuantity(GoodsRange.Name, GoodsRange.QuantityMin + (ClampedScale * (GoodsRange.QuantityMax - GoodsRange.QuantityMin)));
	}
	else
	{
		// Quantity is picked randomly in min-max range
		return FGoodsQuantity(GoodsRange.Name, FMath::FRandRange(GoodsRange.QuantityMin, GoodsRange.QuantityMax));
	}	
}


TArray<FGoodsQuantity> UGoodsFunctionLibrary::GoodsQuantitiesFromRanges(const TArray<FGoodsQuantityRange>& GoodsRanges, const float QuantityScale /* 0.0 - 1.0 */)
{
	TArray<FGoodsQuantity> Goods;
	for (const FGoodsQuantityRange& GoodsRange : GoodsRanges)
	{
		Goods.Add(GoodsQuantityFromRange(GoodsRange, QuantityScale));
	}
	return Goods;
}


FRecipeQuantity UGoodsFunctionLibrary::RecipeQuantityFromRange(const FRecipeQuantityRange& RecipeRange, const float QuantityScale /* 0.0 - 1.0 */)
{
	if (RecipeRange.QuantityMin == RecipeRange.QuantityMax)
	{
		return FRecipeQuantity(RecipeRange.Name, RecipeRange.QuantityMax);
	}
	if (QuantityScale >= 0.0f)
	{
		// Range is in min-max range determined by scale
		float ClampedScale = FMath::Max<float>(QuantityScale, 1.0f);
		return FRecipeQuantity(RecipeRange.Name, RecipeRange.QuantityMin + (ClampedScale * (RecipeRange.QuantityMax - RecipeRange.QuantityMin)));
	}
	else
	{
		return FRecipeQuantity(RecipeRange.Name, FMath::TruncToInt(FMath::RandRange(RecipeRange.QuantityMin, RecipeRange.QuantityMax)));
	}
}


TArray<FRecipeQuantity> UGoodsFunctionLibrary::RecipeQuantitiesFromRanges(const TArray<FRecipeQuantityRange>& RecipeRanges, const float QuantityScale /* 0.0 - 1.0 */)
{
	TArray<FRecipeQuantity> Recipes;
	for (const FRecipeQuantityRange& RecipeRange : RecipeRanges)
	{
		Recipes.Add(RecipeQuantityFromRange(RecipeRange, QuantityScale));
	}
	return Recipes;
}
