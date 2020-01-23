#pragma once
#include "NameCountInt.generated.h"

USTRUCT(BlueprintType)
struct FNameCountInt
{
	GENERATED_BODY()

public:
	FNameCountInt()
	{
		Name = FName();
		Count = 0;
	}

	FNameCountInt(const FName& NewName, const int NewCount)
	{
		Name = NewName;
		Count = NewCount;
	}

	bool operator== (const FNameCountInt& other) const;
	bool operator== (const FNameCountInt& other);
	bool operator== (FNameCountInt& other);

	bool operator== (const FName& otherName) const;
	bool operator== (const FName& otherName);
	bool operator== (FName& otherName);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		int32 Count;

};
