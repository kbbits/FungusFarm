// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ModifiableAttributes.h"
#include "ModifiableAttribute_AC.generated.h"


UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class FUNGUSFARM_API UModifiableAttribute_AC : public UActorComponent
{
	GENERATED_BODY()

public:

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	//	EModifiableAttributeType AttributeType;

	// Identifying code for the type of attribute.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, SaveGame)
		FName AttributeCode;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, SaveGame)
		float BaseValue;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame)
		EModifiableAttributeDisplayType DisplayAs;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame)
		TArray<FModifier> Modifiers;
		
	// Sets default values for this component's properties
	UModifiableAttribute_AC();
	
protected:
	
	UPROPERTY(SaveGame)
	float CurrentValue;

	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ModifiableAttributes")
	float GetBaseValue();

	UFUNCTION(BlueprintCallable, Category = "ModifiableAttributes")
	void SetBaseValue(const float& NewValue);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ModifiableAttributes")
	float GetCurrentValue();

	// Apply an AttributeModifier to this attribute.
	// If the AttributeModifier is a different type than this attribute, it will be ignored.
	UFUNCTION(BlueprintCallable, Category = "ModifiableAttributes")
	void AddModifier(const FAttributeModifier& NewModifier);
	
	// Apply an array of AttributeModifier to this attribute.
	// Any AttributeModifier that is a different type than this attribute will be ignored.
	UFUNCTION(BlueprintCallable, Category = "ModifiableAttributes")
	void AddModifiers(const TArray<FAttributeModifier>& NewModifiers);

	UFUNCTION(BlueprintCallable, Category = "ModifiableAttributes")
	void RemoveModifier(const FAttributeModifier& NewModifier, bool& Successful);

	UFUNCTION(BlueprintCallable, Category = "ModifiableAttributes")
	void ClearModifiers();

	// Recalculates the current value of the attribute, including all modifiers.
	// Should not need to call this directly.  It is called by the various add/set functions.
	UFUNCTION(BlueprintCallable, Category = "ModifiableAttributes")
	void RecalculateValue();
		
};
