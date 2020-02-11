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

	// Compare two strings.
	// Returns 0 if they are equal, negative if first precedes second, positive if second precedes first.
	UFUNCTION(BlueprintCallable, Category = "String")
		static void CompareStrings(const FString first, const FString second, int32& relation, const bool CaseSensitive = false);

};