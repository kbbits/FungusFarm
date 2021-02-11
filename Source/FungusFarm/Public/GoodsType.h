#pragma once

#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"
#include "GoodsType.generated.h"

USTRUCT(BlueprintType)
struct FGoodsType : public FTableRowBase
{
	GENERATED_BODY()

public:
	// Unique name (code) for this goods type.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TAssetPtr<UTexture2D> Thumbnail;

	// If this goods type represents equipment, this is its equipment name.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		FName EquipmentName;
	
	// Currently unused. May be used to group similar goods.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		FName GoodsParentType;

	// If this value is present it will be used during auto-value calculation instead of calculating it.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		float OverrideSellValue;

	// If this value is present it will be used during auto-value calculation instead of calculating it.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		float OverrideBuyCost;

	// Category of goods. Ex: Resource, Cap, Spores, Equipment, EquipmentMod, etc.
	// Also allows other specifiers:
	//    ValueIgnoreProducedFrom:<InputGoodsName> -- Ignore the given input goods (do not recurse it) when calculating auto-values. Prevents infinite loops with circular recipes.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TArray<FName> GoodsCategories;

public:

	bool operator==(const FGoodsType& Other) const
	{
		if (Name != Other.Name) return false;
		if (EquipmentName != Other.EquipmentName) return false;
		return true;
	}
};