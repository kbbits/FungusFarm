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


TArray<FGoodsQuantity> UGoodsFunctionLibrary::MultiplyGoodsQuantities(const TArray<FGoodsQuantity>& GoodsQuantities, const float Multiplier, const bool bTruncateResultQuantities)
{
	TArray<FGoodsQuantity> Results;
	if (Multiplier == 1.0f) { return GoodsQuantities; }
	for (FGoodsQuantity TmpQuantity : GoodsQuantities)
	{
		if (bTruncateResultQuantities)	
		{
			Results.Add(FGoodsQuantity(TmpQuantity.Name, FMath::TruncToFloat(TmpQuantity.Quantity * Multiplier)));
		}
		else 
		{
			Results.Add(FGoodsQuantity(TmpQuantity.Name, TmpQuantity.Quantity * Multiplier));
		}
	}
	return Results;
}


TArray<FGoodsQuantity> UGoodsFunctionLibrary::ApplyGoodsQuantityFactor(const TArray<FGoodsQuantity>& GoodsQuantities, const float Multiplier)
{
	TArray<FGoodsQuantity> Results;
	if (Multiplier == 1.0f) { return GoodsQuantities; }
	for (FGoodsQuantity TmpQuantity : GoodsQuantities)
	{
		float NewQuantity = FMath::CeilToFloat(TmpQuantity.Quantity * Multiplier);
		if (NewQuantity < 1.0f) {
			if (TmpQuantity.Quantity > 0.f) {
				NewQuantity = 1.0f;
			}
			else {
				NewQuantity = 0.0f;
			}
		}
		Results.Add(FGoodsQuantity(TmpQuantity.Name, NewQuantity));		
	}
	return Results;
}


TArray<FGoodsQuantity> UGoodsFunctionLibrary::SumGoodsQuantities(const TArray<FGoodsQuantity>& GoodsOne, const TArray<FGoodsQuantity>& GoodsTwo, bool& bSuccessful, const bool bAllowNegativeTotals, const bool bIncludeZeros)
{
	TMap<FName, FGoodsQuantity> ResultQuantities;
	TArray<FName> UncheckedQuantities;
	float ItemNetSum;
	for (FGoodsQuantity TmpQuantity : GoodsOne)
	{
		if (TmpQuantity.Quantity != 0.0f || bIncludeZeros)
		{
			ResultQuantities.Add(TmpQuantity.Name, TmpQuantity);
			UncheckedQuantities.AddUnique(TmpQuantity.Name);
		}
	}
	for (FGoodsQuantity TmpQuantity : GoodsTwo)
	{
		if (ResultQuantities.Contains(TmpQuantity.Name))
		{
			ItemNetSum = ResultQuantities[TmpQuantity.Name].Quantity + TmpQuantity.Quantity;
			if (!bAllowNegativeTotals && ItemNetSum < 0.0f)
			{
				bSuccessful = false;
				return TArray<FGoodsQuantity>();
			}
			if (ItemNetSum != 0.0f || bIncludeZeros)
			{
				ResultQuantities[TmpQuantity.Name].Quantity = ItemNetSum;
			}
			else 
			{
				ResultQuantities.Remove(TmpQuantity.Name);
			}
			UncheckedQuantities.Remove(TmpQuantity.Name);
		}
		else {
			if (!bAllowNegativeTotals && TmpQuantity.Quantity < 0.0f)
			{
				bSuccessful = false;
				return TArray<FGoodsQuantity>();
			}
			if (TmpQuantity.Quantity != 0.0f || bIncludeZeros)
			{
				ResultQuantities.Add(TmpQuantity.Name, TmpQuantity);
			}			
		}
	}
	if (!bAllowNegativeTotals) {
		for (FName TmpName : UncheckedQuantities)
		{
			if (ResultQuantities[TmpName].Quantity < 0.0f) {
				bSuccessful = false;
				return TArray<FGoodsQuantity>();
			}
		}
	}
	TArray<FGoodsQuantity> FinalResults;
	ResultQuantities.GenerateValueArray(FinalResults);
	bSuccessful = true;
	return FinalResults;
}


TArray<FGoodsQuantity> UGoodsFunctionLibrary::SubtractGoodsQuantities(const TArray<FGoodsQuantity>& GoodsOne, const TArray<FGoodsQuantity>& GoodsToSubtract, bool& bSuccessful, const bool bAllowNegativeTotals)
{
	return SumGoodsQuantities(GoodsOne, MultiplyGoodsQuantities(GoodsToSubtract, -1.0f, false), bSuccessful, bAllowNegativeTotals);
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
		float SafeMin = TmpRange.QuantityMin > 0.f ? 1.0f : 0.0f;
		float SafeMax = TmpRange.QuantityMax > 0.f ? 1.0f : 0.0f;
		if (bTruncateQuantitiesToInteger) {
			ResultDropChance.GoodsOdds.Add(FGoodsQuantityRange(TmpRange.Name, FMath::Max(FMath::FloorToFloat(TmpRange.QuantityMin * Multiplier), SafeMin), FMath::Max(FMath::RoundToFloat(TmpRange.QuantityMax * Multiplier), SafeMax)));
		}
		else {
			ResultDropChance.GoodsOdds.Add(FGoodsQuantityRange(TmpRange.Name, FMath::Max(TmpRange.QuantityMin * Multiplier, SafeMin), FMath::Max(TmpRange.QuantityMax * Multiplier, SafeMax)));
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
