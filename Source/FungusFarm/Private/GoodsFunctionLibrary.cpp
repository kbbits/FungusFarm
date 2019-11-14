// Fill out your copyright notice in the Description page of Project Settings.


#include "GoodsFunctionLibrary.h"

FGoodsQuantity UGoodsFunctionLibrary::GoodsQuantityFromRange(const FGoodsQuantityRange& GoodsRange, const float QuantityScale /* 0.0 - 1.0 */)
{
	UE_LOG(LogFFGame, Warning, TEXT("QuantityFromRange for %s"), *GoodsRange.Name.ToString());
	if (GoodsRange.QuantityMin == GoodsRange.QuantityMax)
	{
		return FGoodsQuantity(GoodsRange.Name, GoodsRange.QuantityMax);
	}
	if (QuantityScale >= 0.0f)
	{
		UE_LOG(LogFFGame, Warning, TEXT("  Quantity scale raw: %f"), QuantityScale);
		// Range is in min-max range determined by scale
		float ClampedScale = FMath::Min<float>(QuantityScale, 1.0f);
		float PickedQuantity = FMath::TruncToFloat(GoodsRange.QuantityMin + (ClampedScale * (GoodsRange.QuantityMax - GoodsRange.QuantityMin)));
		UE_LOG(LogFFGame, Warning, TEXT("  QuantityFromRange clamped scale: %f picked quantity: %f"), ClampedScale, PickedQuantity);
		return FGoodsQuantity(GoodsRange.Name, PickedQuantity);
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
		float ClampedScale = FMath::Min<float>(QuantityScale, 1.0f);
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


TArray<FGoodsQuantity> UGoodsFunctionLibrary::EvaluateGoodsDrop(const FGoodsDropChance& DropChance, const float QuantityScale /* 0.0 - 1.0 */)
{
	TArray<FGoodsQuantity> EvaluatedGoods;
	float PickChance;
	PickChance = FMath::FRandRange(0.0f, DropChance.PercentChance);
	if (PickChance <= DropChance.PercentChance)
	{
		for (const FGoodsQuantityRange& QuantityRange : DropChance.GoodsOdds)
		{
			FGoodsQuantity Quantity = GoodsQuantityFromRange(QuantityRange, QuantityScale);
			if (Quantity.Quantity > 0.0f)
			{
				EvaluatedGoods.Add(Quantity);
			}
		}		
	}
	return EvaluatedGoods;
}


TArray<FGoodsQuantity> UGoodsFunctionLibrary::EvaluateGoodsDrops(const TArray<FGoodsDropChance>& DropChances, const float QuantityScale)
{
	TArray<FGoodsQuantity> EvaluatedGoods;
	for (const FGoodsDropChance& DropChance : DropChances)
	{
		EvaluatedGoods.Append(EvaluateGoodsDrop(DropChance, QuantityScale));
	}
	return EvaluatedGoods;
}


TArray<FGoodsDropChance> UGoodsFunctionLibrary::FlattenToEvaluatedGoods(const TArray<FGoodsDropChance>& DropChances, const float QuantityScale /* 0.0 - 1.0 */)
{
	TArray<FGoodsDropChance> Flattened;
	for (const FGoodsDropChance& DropChance : DropChances)
	{
		FGoodsDropChance NewChance;
		NewChance.DropGroup = DropChance.DropGroup;
		NewChance.PercentChance = 1.0f;
		TArray<FGoodsQuantity> GoodsQuantities = EvaluateGoodsDrop(DropChance, QuantityScale);
		for (FGoodsQuantity GoodsQuantity : GoodsQuantities)
		{
			NewChance.GoodsOdds.Add(FGoodsQuantityRange(GoodsQuantity.Name, GoodsQuantity.Quantity, GoodsQuantity.Quantity));
		}
		Flattened.Add(NewChance);
	}
	return Flattened;
}
