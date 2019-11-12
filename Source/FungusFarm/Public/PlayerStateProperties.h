#pragma once

#include "Engine/DataTable.h"
#include "PlayerStateProperties.generated.h"

USTRUCT(BlueprintType)
struct FPlayerStateProperties : public FTableRowBase
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, Category = "FFPlayerState")
		float ExperiencePoints;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, Category = "FFPlayerState")
		int32 ExperienceLevel;
};

