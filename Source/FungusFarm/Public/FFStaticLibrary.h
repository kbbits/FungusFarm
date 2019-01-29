#pragma once

//#include "CoreMinimal.h"
#include "ModifiableAttributes.h"
#include "FFStaticLibrary.generated.h"

UCLASS()
class UFFStaticLibrary : public UObject
{
	GENERATED_UCLASS_BODY()

public:

	//FORCEINLNE function
	static FORCEINLINE bool IsValid(AActor* TheActor)
	{
		if (!TheActor) return false;
		if (!TheActor->IsValidLowLevel()) return false;
		return true;
	}

	UFUNCTION(BlueprintCallable, Category="ModifiableAttributes")
	static void SumModifiers(const TArray<FModifier>& ModsToSum, float& ScalarTotal, float& MultiplierTotal);

};