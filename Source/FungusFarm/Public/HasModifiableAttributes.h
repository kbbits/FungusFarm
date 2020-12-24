#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ModifiableAttribute_AC.h"
#include "HasModifiableAttributes.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHasModifiableAttributes : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class FUNGUSFARM_API IHasModifiableAttributes
{
	GENERATED_BODY()

		// Add interface functions to this class. This is the class that will be inherited to implement this interface.

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ModifiableAttributes")
		TArray<UModifiableAttribute_AC*> GetModifiableAttributes();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ModifiableAttributes")
		UModifiableAttribute_AC * GetModifiableAttribute(const FName AttributeCode);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ModifiableAttributes")
		void RefreshAttributeModifierCalculations();
};