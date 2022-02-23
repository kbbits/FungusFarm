#pragma once

#include "Engine/DataTable.h"
#include "GameType.generated.h"

USTRUCT(BlueprintType)
struct FGameDifficultySettings
{
	GENERATED_BODY()

public:

	/** Multiplier for time it takes to grow crops.
	 *  ex: 2.0 = crops take 2x longer to grow. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		float CropGrowthFactor = 1.0f;

	/** Multiplier applied to goods produced from harvesting a crop. Does not apply to sprinkle goods.
	 *  Does not apply to harvest bonus goods.
	 *  Will not reduce non-zero quantities below 1. Minimum quantities are rounded down, maximum quantities are rounded to nearest integer. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		float CropHarvestFactor = 1.0f;

	/** Multiplier for time it takes to craft a recipe.
	 *   ex: 2.0 = recipes take twice as long to craft. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		float CraftingTimeFactor = 1.0f;

	/** Multiplier on the quanity of each ingredient need to craft a given recipe. Quanitities will not be reduced below 1. All fractions are rounded up.
	 *   ex: 0.5 = all recipes require half the normal quantity of ingredients in order to craft them.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		float CraftingIngredientsFactor = 1.0f;

	/** Multiplier on the quantity of goods produced by crafting recipes. A non-zero quantity will not be reduced below 1. 
	 *  Minimum quantities are rounded down, maximum quantities are rounded to nearest integer.
	 *   ex: 2.0 = all recipes produce twice the normal quantity of output goods. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		float CraftingResultsFactor = 1.0f;

	/** Multiplies the amount of experience required for player to level up each level.
	 *  ex: 2.0 = twice as much experience is required for each level up. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		float ExperienceRequiredFactor = 1.0f;

	/** The exponent used in the required experience calculation for each level up.
	 *  Default = 2.25
	 *  formula: ExperienceRequiredFactor * ((FMath::TruncToFloat((FMath::Pow(CurLevel + FMath::FloorToInt(Level / 5.0f), ExperienceRequiredExponent) * 10.0f) + (CurLevel * 5)) * 100.0f) + 1200.0f) \
	 *  For levels 11+ Additional xp required = normal formula for lower level + ((level - 10)^(1 + (0.1 *(level - 11))))
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		float ExperienceRequiredExponent = 2.25f;

	/** Whether goods gathered from clicking crops count towards harvest counts. 
	 *  Default = false.  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		bool bCropClicksCountAsHarvest = false;
};


USTRUCT(BlueprintType)
struct FGameType : public FTableRowBase
{
	GENERATED_BODY()

public:

	/** The internal name of this game type. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		FName Name;

	/** Name displayed to use for this game type. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		FText DisplayName;

	/** Difficulty settings associated with this game type. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		FGameDifficultySettings DifficultySettings;
};