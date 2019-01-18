#pragma once

#include "Engine/DataTable.h"
#include "CraftingSlot.generated.h"

USTRUCT(BlueprintType)
struct FCraftingSlot : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		int32 Value;
};
