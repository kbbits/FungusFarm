#pragma once

#include "Engine/DataTable.h"
#include "CropPlotInfo.generated.h"

USTRUCT(BlueprintType)
struct FCropPlotInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	// Unique name of this Crop plot type
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		float BaseGrowthRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		float BaseTemperature;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		float BaseMoisture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		float BaseMineralCapacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		float BaseProteinCapacity;
};