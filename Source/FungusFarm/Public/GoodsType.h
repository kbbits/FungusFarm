#pragma once

#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"
#include "GoodsType.generated.h"

USTRUCT(BlueprintType)
struct FGoodsType : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TAssetPtr<UTexture2D> Thumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		FName EquipmentName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		FName GoodsParentType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		float SellValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		float BuyCost;
};