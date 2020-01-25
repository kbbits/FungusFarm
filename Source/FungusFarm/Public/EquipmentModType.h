#pragma once

#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"
#include "ModifiableAttributes.h"
#include "EquipmentModType.generated.h"

UENUM(BlueprintType)
enum class EEquipmentModDuration : uint8
{
	// Always in effect when equipped.
	Constant		UMETA(DisplayName = "Constant"),
	// Currrently unused
	Instant		 	UMETA(DisplayName = "Instant"),
	// Active for Duration of seconds, then destroy self
	Duration	 	UMETA(DisplayName = "Duration"),
	// Activate for Duration seconds, re-activates every DurationInterval seconds. After MaxActivations, destroy self.
	Repeating		UMETA(DisplayName = "Repeating"),
	// Active until activation event occurs MaxActivation times
	EventLimited	UMETA(DisplayName = "Event Limited"),
	// Only active when activation event occurs. Then active for Duration seconds. After duration expires MaxActivation times, destroy self.
	EventTriggered	UMETA(DisplayName = "Event Triggered")
};


UENUM(BlueprintType)
enum class EEquipmentModEvent : uint8
{
	// Crop related events
	CropPlanted		UMETA(DisplayName = "CropPlanted"),
	ReadyToHarvest	UMETA(DisplayName = "Ready to Harvest"),
	Harvested		UMETA(DisplayName = "Harvested"),
};


USTRUCT(BlueprintType)
struct FEquipmentModType : public FTableRowBase
{
	GENERATED_BODY()

public:
	// The unique name of this equipment mod (usually the same as equipment name for this mod - equipment mods are also equipment)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame)
		FName Name;

	// If this is true, this equipment mod will be destroyed when it is removed from equipment.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame)
		bool bDestroyOnRemove;

	// Describes when and for how long this mod is effective.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame)
		EEquipmentModDuration DurationType;

	// Value <= 0 means infinite.  Specifics for DurationType:
	//   = Constant, Instant and EventLimited - ignored.
	//   = Duration - number of seconds mod is active before it expires.
	//   = Repeating - how long each repeated activation lasts.
	//   = EventTriggered - how long each triggered event lasts.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame)
		float Duration;

	// Value must be > 0.01f.  Specifics for Duration Type:
	//   = Constant, Instant, Duration, EventLimited, EventTriggered - ignored.
	//   = Repeating - number of seconds between each repeated activation.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame)
		float DurationInterval;

	// Value <= 0 means infinite. Relevant to: Repeated, EventTriggered and EventLimited
	// The maximum number of repeated or triggered events before the mod equipment expires.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame)
		int32 MaximumActivations;

	// For EventXxxx DurationType events, this indicates what event we are associated with.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame)
		EEquipmentModEvent ActivationEvent;

	// The modifiers this mod equipment applies when active.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame)
		TArray<FAttributeModifier> Modifiers;

};