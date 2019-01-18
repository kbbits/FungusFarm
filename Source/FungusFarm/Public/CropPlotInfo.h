#pragma once

#include "Engine/DataTable.h"
#include "CropPlotInfo.generated.h"

USTRUCT(BlueprintType)
struct FCropPlotInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		float BaseGrowthRate;
};