#pragma once

#include "Engine/DataTable.h"
#include "PlayerStateProperties.generated.h"

USTRUCT(BlueprintType)
struct FPlayerStateProperties : public FTableRowBase
{
	GENERATED_BODY()

public:
	
	// Player's current experience points.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, Category = "FFPlayerState")
		float ExperiencePoints;

	// Player's current level.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, Category = "FFPlayerState")
		int32 ExperienceLevel;

	// Map of level names to the index of the last navigation target anchor index the player used for the given (sub)level.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, Category = "FFPlayerState")
		TMap<FName, int32> LastAnchorIndexForLevel;
};

