#pragma once

#include "Engine/DataTable.h"
#include "GameplayGoalProviderTemplate.generated.h"

USTRUCT(BlueprintType)
struct FGameplayGoalProviderTemplate : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		float DelayBetweenNewGoalsMinimum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		float DelayBetweenNewGoalsMaximum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		int32 MaximumCurrentGoals;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		FText FriendlyName;
};