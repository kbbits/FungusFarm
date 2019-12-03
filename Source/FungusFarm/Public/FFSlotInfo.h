// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerStateProperties.h"
#include "SlotInfo.h"
#include "FFSlotInfo.generated.h"

/**
 * 
 */
UCLASS()
class FUNGUSFARM_API UFFSlotInfo : public USlotInfo
{
	GENERATED_BODY()

public:
	UFFSlotInfo() : Super()
	{}

	UPROPERTY(BlueprintReadWrite, Category = "SlotInfo")
	FPlayerStateProperties PlayerProperties;

	UPROPERTY(BlueprintReadOnly, Category = "SlotInfo")
	TArray<FName> SecondaryGoalProviders;
	
};
