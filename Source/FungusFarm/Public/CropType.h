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
	// The unique name for this crop type
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		FName Name;

	// User-friendly name
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TSoftObjectPtr<UTexture2D> Thumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		UTexture2D* ThumbnailHardRef;
	
	// The actor class that represents this crop
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TSubclassOf<AActor> ActorClass;

	// Costs to plant one of these crops
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TArray<FGoodsQuantity> PlantingCost;

	// Goods awarded to player when crop is harvested
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TArray<FGoodsDropChance> HarvestGoods;

	// Additional goods awarded to player when crop is harvested. These don't appear on crop info menus.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TArray<FGoodsDropChance> HarvestGoodsBonus;

	// Goods drops when player gets sprinkles from this crop. (when player clicks the crop)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TArray<FGoodsDropChance> SprinkleGoods;

	// How long (in seconds) this crop takes to grow (when growth rate is 1.0)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		float GrowthTime;

	// Temperature (degrees celcius) that crop grows best
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		float IdealTemperature;

	// Range of viable temp is IdealTemperature +/- this amount
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		float TemperatureToleraceRange;

	// Moisure level (0.0 - 1.0) that crop grows best
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		float IdealMoisture;

	// Range of viable moisture is IdealMoisture +/- this amount
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		float MoistureToleraceRange;

	// The minimum MineralCapacity for this crop to grow
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		float RequiredMineralCapacity;

	// The minimum ProteinCapacity for this crop to grow
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		float RequiredProteinCapacity;

	// Experience awarded to player for harvesting this crop type
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		float HarvestExperience;

	// Goods removed from an inventory each second while the crop is growing.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TArray<FGoodsQuantity> GrowthInputRequirements;
};