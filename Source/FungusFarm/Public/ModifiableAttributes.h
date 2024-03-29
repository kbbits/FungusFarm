#pragma once
#include "Engine/DataTable.h"
//#include "TextUtil.h"
#include "ModifiableAttributes.generated.h"


UENUM(BlueprintType)
enum class EModifierType : uint8
{
	Scalar		UMETA(DisplayName = "Scalar"),
	Multiplier 	UMETA(DisplayName = "Multiplier")
};

UENUM(BlueprintType)
enum class EModifiableAttributeDisplayType : uint8
{
	PlainValue	UMETA(DisplayName = "PlainValue"),
	Percent 	UMETA(DisplayName = "Percent"),
	Boolean		UMETA(DisplayName = "Boolean")
};


USTRUCT(BlueprintType)
struct FModifier
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		EModifierType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		float Value;

	FModifier()
	{
		Type = EModifierType::Multiplier;
		Value = 0.0f;
	}

	bool operator==(const FModifier& Other) const
	{
		if (Type != Other.Type) return false;
		if (Value != Other.Value) return false;
		return true;
	}
};

/*
	Represents a modifier that could be applied to an attriubute. (applied to a ModifiableAttribute)
*/
USTRUCT(BlueprintType)
struct FAttributeModifier
{
	GENERATED_BODY()

public:
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	//	EModifiableAttributeType AttributeType;

	// Identifying code for the type of attribute being modified.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		FName AttributeCode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		FModifier Modifier;

	FAttributeModifier()
	{
		//AttributeType = EModifiableAttributeType::GrowthRate;
		AttributeCode = FName("GrowthRate");
	}

	bool operator==(const FAttributeModifier& Other) const
	{
		//if (AttributeType != Other.AttributeType) return false;
		if (AttributeCode != Other.AttributeCode) return false;
		if (!(Modifier == Other.Modifier)) return false;
		return true;
	}
};

/*
	Represents an attribute on an object (usually an Actor).
*/
USTRUCT(BlueprintType)
struct FModifiableAttribute
{
	GENERATED_BODY()

public:
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	//	EModifiableAttributeType AttributeType;

	// Identifying code for the type of attribute.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		FName AttributeCode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		float BaseValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		float ModifiedValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TArray<FModifier> Modifiers;

	FModifiableAttribute()
	{
		//AttributeType = EModifiableAttributeType::GrowthRate;
		AttributeCode = FName();
		BaseValue = 0.0f;
		ModifiedValue = 0.0f;
	}

	bool operator==(const FModifiableAttribute& Other) const
	{
		//if (AttributeType != Other.AttributeType) return false;
		if (AttributeCode != Other.AttributeCode) return false;
		if (BaseValue != Other.BaseValue) return false;
		if (ModifiedValue != Other.ModifiedValue) return false;
		return true;
	}
};

/*
	Named sets of modifiers that are stored and added to in-game objects that will apply modifiers to others.
	Lists of these are intented to be kept in a DataTable and will describe the modifier effects available for use in-game.
	Ex: a CropPlotMod would have one of these which describes the modifers applied when the CropPlotMod is added to a CropPlot.
*/
USTRUCT(BlueprintType)
struct FNamedModifierSet : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TArray<FAttributeModifier> Modifiers;

	bool operator==(const FNamedModifierSet& Other) const
	{
		if (Name != Other.Name) return false;
		if (Modifiers.Num() != Other.Modifiers.Num()) return false;
		for (int32 i = 0; i < Modifiers.Num(); i++)
		{
			if (!(Modifiers[i] == Other.Modifiers[i])) return false;
		}
		return true;
	}
};
