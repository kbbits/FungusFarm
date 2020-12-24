// Fill out your copyright notice in the Description page of Project Settings.


#include "GoodsFunctionLibrary.h"

FGoodsQuantity UGoodsFunctionLibrary::GoodsQuantityFromRange(const FGoodsQuantityRange& GoodsRange, const float QuantityScale /* 0.0 - 1.0 */)
{
	//UE_LOG(LogFFGame, Warning, TEXT("QuantityFromRange for %s"), *GoodsRange.Name.ToString());
	// Check if min == max
	if (GoodsRange.QuantityMin == GoodsRange.QuantityMax)
	{
		return FGoodsQuantity(GoodsRange.Name, GoodsRange.QuantityMax);
	}
	// If QuantityScale is >= 0.0, then we determine the quantity by selecting a value that is QuantityScale % between min and max values.
	if (QuantityScale >= 0.0f)
	{
		//UE_LOG(LogFFGame, Warning, TEXT("  Quantity scale raw: %f"), QuantityScale);
		// Range is in min-max range determined by scale
		float ClampedScale = FMath::Min<float>(QuantityScale, 1.0f);
		float PickedQuantity = FMath::TruncToFloat(GoodsRange.QuantityMin + (ClampedScale * (GoodsRange.QuantityMax - GoodsRange.QuantityMin)));
		//UE_LOG(LogFFGame, Warning, TEXT("  QuantityFromRange clamped scale: %f picked quantity: %f"), ClampedScale, PickedQuantity);
		return FGoodsQuantity(GoodsRange.Name, PickedQuantity);
	}
	else
	{
		// Quantity is picked randomly in min-max range
		return FGoodsQuantity(GoodsRange.Name, FMath::TruncToFloat(FMath::FRandRange(GoodsRange.QuantityMin, GoodsRange.QuantityMax)));
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
		return FRecipeQuantity(RecipeRange.Name, FMath::TruncToInt(RecipeRange.QuantityMin + (ClampedScale * (RecipeRange.QuantityMax - RecipeRange.QuantityMin))));
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


TArray<FGoodsDropChance> UGoodsFunctionLibrary::MultiplyGoodsDropChanceQuantities(const TArray<FGoodsDropChance>& DropChances, const float Multiplier, const bool bTruncateQuantitiesToInteger)
{
	TArray<FGoodsDropChance> ResultDropChances;

	if (Multiplier == 1.0f) { return DropChances; }
	for (FGoodsDropChance TmpDropChance : DropChances)
	{
		ResultDropChances.Add(MultiplyGoodsDropChanceQuantity(TmpDropChance, Multiplier, bTruncateQuantitiesToInteger));
	}
	return ResultDropChances;
}


FGoodsDropChance UGoodsFunctionLibrary::MultiplyGoodsDropChanceQuantity(const FGoodsDropChance& DropChance, const float Multiplier, const bool bTruncateQuantitiesToInteger)
{
	FGoodsDropChance ResultDropChance = DropChance;

	if (Multiplier == 1.0f) { return DropChance; }
	ResultDropChance.GoodsOdds.Empty(DropChance.GoodsOdds.Num());
	for (FGoodsQuantityRange TmpRange : DropChance.GoodsOdds)
	{
		if (bTruncateQuantitiesToInteger) {
			ResultDropChance.GoodsOdds.Add(FGoodsQuantityRange(TmpRange.Name, FMath::FloorToFloat(TmpRange.QuantityMin * Multiplier), FMath::FloorToFloat(TmpRange.QuantityMax * Multiplier)));
		}
		else {
			ResultDropChance.GoodsOdds.Add(FGoodsQuantityRange(TmpRange.Name, TmpRange.QuantityMin * Multiplier, TmpRange.QuantityMax * Multiplier));
		}
	}
	return ResultDropChance;
}


TArray<FGoodsQuantity> UGoodsFunctionLibrary::EvaluateGoodsDrop(const FGoodsDropChance& DropChance, const float QuantityScale /* 0.0 - 1.0 */)
{
	TArray<FGoodsQuantity> EvaluatedGoods;
	float PercentChance = DropChance.PercentChance;
	float PickChance;
	if (PercentChance <= 0.0f)
	{
		return EvaluatedGoods;
	}
	if (PercentChance > 1.0f) { PercentChance = 1.0f; }
	
	PickChance = FMath::FRand(); // 0.0 - 1.0
	if (PickChance <= PercentChance)
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
