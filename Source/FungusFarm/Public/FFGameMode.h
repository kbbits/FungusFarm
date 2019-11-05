// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameplayGoal.h"
#include "GameplayGoalProvider.h"
#include "GoalsProviderComponent.h"
#include "FFGameMode.generated.h"

/**
 * 
 */
UCLASS()
class FUNGUSFARM_API AFFGameMode : public AGameMode 
{
	GENERATED_BODY()

public:
	AFFGameMode(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, Category = "Components")
		UGoalsProviderComponent* GameplayGoalsProvider = nullptr;

protected:

	virtual void BeginPlay() override;

public:
	
	
	
};
