// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "Engine/DataTable.h"
#include "RecipeQuantity.generated.h"

USTRUCT(BlueprintType)
struct FRecipeQuantity //: public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		float Quantity;

public:
	FRecipeQuantity()
	{
		Name = FName();
		Quantity = 0.0f;
	}

	FRecipeQuantity(const FName& NewName, const float NewQuantity)
	{
		Name = NewName;
		Quantity = NewQuantity;
	}

	bool operator==(const FRecipeQuantity& Other) const
	{
		if (Name != Other.Name) return false;
		if (Quantity != Other.Quantity) return false;
		return true;
	}
};
