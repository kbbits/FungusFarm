#pragma once

#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"
#include "EquipmentType.generated.h"

USTRUCT(BlueprintType)
struct FEquipmentType : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TSoftObjectPtr<UTexture2D> Thumbnail;

	// The actor class associated with this equipment.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TSubclassOf<AActor> ActorClass;

	// Used by the actor class of this equipment.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		FName SubtypeName;

};