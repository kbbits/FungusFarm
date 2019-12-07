#pragma once

#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"
#include "GameplayGoalProviderTemplate.generated.h"

USTRUCT(BlueprintType)
struct FGameplayGoalProviderTemplate : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "Gameplay Goals")
		TAssetPtr<UTexture2D> ProviderImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "Gameplay Goals")
		float DelayBetweenNewGoalsMinimum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "Gameplay Goals")
		float DelayBetweenNewGoalsMaximum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "Gameplay Goals")
		int32 MaximumCurrentGoals;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "Gameplay Goals")
		FText FriendlyName;
};