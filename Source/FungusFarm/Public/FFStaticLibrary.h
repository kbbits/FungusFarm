#pragma once

#include "CoreMinimal.h"
#include <HAL/PlatformFile.h>
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ModifiableAttributes.h"
#include "FFStaticLibrary.generated.h"

/** Used to find next available slot id */
class FFFindSlotVisitor : public IPlatformFile::FDirectoryVisitor
{
public:
	TArray<FString>& FoundProfiles;
	TArray<int32>& FoundSlotIds;

	FFFindSlotVisitor(TArray<FString>& FoundProfiles, TArray<int32>& FoundSlotIds)
		: FoundProfiles(FoundProfiles), FoundSlotIds(FoundSlotIds)
	{}

	virtual bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory) override;
};

UCLASS()
class FUNGUSFARM_API UFFStaticLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

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

	UFUNCTION(BlueprintCallable, Category = "Save Game Profiles")
	static void FindSaveProfileFileNames(TArray<FString>& FoundProfileFilenames, TArray<int32>& FoundSlotIds);
};