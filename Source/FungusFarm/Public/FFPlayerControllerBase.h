// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayGoal.h"
#include "GameplayGoalListener.h"
#include "ActorGoalsComponent.h"
#include "FFPlayerControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class FUNGUSFARM_API AFFPlayerControllerBase : public APlayerController, public IGameplayGoalListener
{
	GENERATED_BODY()

public:
	AFFPlayerControllerBase(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, Category = "Gameplay Goals")
		UActorGoalsComponent* GameplayGoalsManager = nullptr;

protected:
	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay Goals")
		void OnNewGameplayGoals(const TArray<FGameplayGoal>& NewGoals);
	virtual void OnNewGameplayGoals_Implementation(const TArray<FGameplayGoal>& NewGoals) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay Goals")
		void OnGameplayGoalsCompleted(const TArray<FGameplayGoal>& CompletedGoals);
	virtual void OnGameplayGoalsCompleted_Implementation(const TArray<FGameplayGoal>& CompletedGoals) override;
};
