// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataTable.h"
#include "GoodsQuantity.h"
#include "GoodsQuantityRange.generated.h"

USTRUCT(BlueprintType)
struct FGoodsQuantityRange : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		float QuantityMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		float QuantityMax;

public:

	FGoodsQuantityRange()
	{
		Name = FName::FName();
		QuantityMin = 0.0f;
		QuantityMax = 0.0f;
	}

	FGoodsQuantityRange(const FName NewName, const float Min, const float Max)
	{
		Name = NewName;
		QuantityMin = Min;
		QuantityMax = Max;
	}

	bool operator==(const FGoodsQuantityRange& Other) const
	{
		if (Name != Other.Name) return false;
		if (QuantityMin != Other.QuantityMin) return false;
		if (QuantityMax != Other.QuantityMax) return false;
		return true;
	}
};
