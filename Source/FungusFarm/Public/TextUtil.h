#pragma once

#include "TextUtil.generated.h"

/**
 *
 */
UCLASS()
class FUNGUSFARM_API UTextUtil : public UObject
{
	GENERATED_BODY()

public:

	//UFUNCTION(BlueprintCallable)
	//FString GetEnumValueAsString(const FString& Name, TEnum Value);
};

//#define EnumToString(EnumClassName, ValueOfEnum) UTextUtil::GetEnumValueAsString<EnumClassName>(FString(TEXT(#EnumClassName)), (ValueOfEnum))