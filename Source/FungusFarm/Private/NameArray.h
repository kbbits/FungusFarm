#pragma once
#include "NameArray.generated.h"

USTRUCT(BlueprintType)
struct FNameArray 
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TArray<FName> Names;

};
