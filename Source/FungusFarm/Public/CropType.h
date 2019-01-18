#pragma once

#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"
#include "GoodsQuantity.h"
#include "GoodsDropChance.h"
#include "CropType.generated.h"

USTRUCT(BlueprintType)
struct FCropType : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TSoftObjectPtr<UTexture2D> Thumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		UTexture2D* ThumbnailHardRef;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TSubclassOf<AActor> ActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TArray<FGoodsQuantity> PlantingCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TArray<FGoodsDropChance> HarvestGoods;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TArray<FGoodsDropChance> HarvestGoodsBonus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TArray<FGoodsDropChance> SprinkleGoods;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		float GrowthTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TArray<FGoodsQuantity> GrowthInputRequirements;
};