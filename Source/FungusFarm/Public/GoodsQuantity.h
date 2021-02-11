// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "Engine/DataTable.h"
#include "GoodsQuantity.generated.h"

USTRUCT(BlueprintType)
struct FGoodsQuantity //: public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		float Quantity;

public:
	FGoodsQuantity()
	{
		Name = FName();
		Quantity = 0.0f;
	}

	FGoodsQuantity(const FName& NewName, const float NewQuantity)
	{
		Name = NewName;
		Quantity = NewQuantity;
	}

	bool operator==(const FGoodsQuantity& Other) const
	{
		if (Name != Other.Name) return false;
		if (Quantity != Other.Quantity) return false;
		return true;
	}
};



USTRUCT(BlueprintType)
struct FGoodsQuantitySet
{
	GENERATED_BODY()

public:

	// Optional if you want to use named sets.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TArray<FGoodsQuantity> GoodsQuantities;

public:

	FGoodsQuantitySet()
	{
		Name = FName();
	}

	FGoodsQuantitySet(FName NewName)
	{
		Name = NewName;
	}

	FGoodsQuantitySet(FName NewName, TArray<FGoodsQuantity>& NewQuantities)
	{
		Name = NewName;
		GoodsQuantities = NewQuantities;
	}

	bool operator==(const FGoodsQuantitySet& Other) const
	{
		if (Name != Other.Name) { return false; }
		if (GoodsQuantities.Num() != Other.GoodsQuantities.Num()) { return false; }
		for (FGoodsQuantity TmpQuantity : GoodsQuantities)
		{ 
			if (Other.GoodsQuantities.Find(TmpQuantity) == INDEX_NONE) { return false; }
		}		
		return true;
	}
};
