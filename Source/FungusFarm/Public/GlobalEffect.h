#pragma once

#include "ModifiableAttributes.h"
#include "GlobalEffect.generated.h"



USTRUCT(BlueprintType)
struct FGlobalEffect
{
	GENERATED_BODY()

public:
	
	// Name of effect for display to user. (when relevant)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		FText DisplayName;

	// Internal, globally unique name of this global effect.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		FName UniqueName;

	// Thumbnail of effect for display to user.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TAssetPtr<UTexture2D> Thumbnail;

	// Modified objects (crop plots, lab equip, etc.) must have all of these in order for the effect to apply.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TArray<FName> RequiredTags;

	// Modified objects (crop plots, lab equip, etc.) must not have any of these in order for the effect to apply.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TArray<FName> DeniedTags;

	// The remaining time for this effect.  <= 0 means infinite duration.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		float RemainingDuration;

	// The modifiers that will apply to objects affected by this effect.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TArray<FAttributeModifier> AttributeModifiers;
};