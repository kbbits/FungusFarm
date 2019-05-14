// Fill out your copyright notice in the Description page of Project Settings.

#include "ModifiableAttribute_AC.h"

// Sets default values for this component's properties
UModifiableAttribute_AC::UModifiableAttribute_AC()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// Init members
	AttributeType = EModifiableAttributeType::GrowthRate;
	BaseValue = 0.0;
	CurrentValue = 0.0;
}


// Called when the game starts
void UModifiableAttribute_AC::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
//void UModifiableAttribute_AC::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//}

// Get the base value of the attribute.
float UModifiableAttribute_AC::GetBaseValue()
{
	return BaseValue;
}

// Set the base value of the attribute.
void UModifiableAttribute_AC::SetBaseValue(const float& NewValue)
{
	BaseValue = NewValue;
	RecalculateValue();
}

// Gets the current value of this attribute with all modifiers applied.
float UModifiableAttribute_AC::GetCurrentValue()
{
	return CurrentValue;
}

/*
  Apply an AttributeModifier to this attribute.  
  If the AttributeModifier is a different type than this attribute, it will be ignored.
*/
void UModifiableAttribute_AC::AddModifier(const FAttributeModifier& NewModifier) 
{
	if (NewModifier.AttributeType == AttributeType) 
	{
		Modifiers.Add(NewModifier.Modifier);
		RecalculateValue();
	}
}

/*
  Apply an array of AttributeModifier to this attribute.
  Any AttributeModifier that is a different type than this attribute will be ignored.
*/
void UModifiableAttribute_AC::AddModifiers(const TArray<FAttributeModifier>& NewModifiers)
{
	if (NewModifiers.Num() == 0)
		return;

	for (FAttributeModifier AttrMod : NewModifiers)
	{
		if (AttrMod.AttributeType == AttributeType)
		{
			Modifiers.Add(AttrMod.Modifier);
		}
	}
	RecalculateValue();
}

// Remove all modifiers from this attribute.
void UModifiableAttribute_AC::ClearModifiers() 
{
	Modifiers.Empty();
	CurrentValue = BaseValue;
}

// Recalculates the current value of the attribute, including all modifiers.
// Should not need to call this directly.  It is called by the various add/set functions.
void UModifiableAttribute_AC::RecalculateValue() 
{
	float ScalarTotal = 0;
	float MultiplierTotal = 0;

	UFFStaticLibrary::SumModifiers(Modifiers, ScalarTotal, MultiplierTotal);
	CurrentValue = (BaseValue * (1.0 + MultiplierTotal)) + ScalarTotal;
}

