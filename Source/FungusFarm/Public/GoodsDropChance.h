// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataTable.h"
#include "GoodsQuantityRange.h"
#include "GoodsDropChance.generated.h"

USTRUCT(BlueprintType)
struct FGoodsDropChance : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		float PercentChance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		int DropGroup;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TArray<FGoodsQuantityRange> GoodsOdds;
};