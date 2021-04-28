// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayGoal.h"
#include "GameplayGoalListener.h"
#include "ActorGoalsComponent.h"
#include "NameArray.h"
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

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay Goals")
		void OnGameplayGoalAbandoned(const FName& AbandonedGoalName);
	virtual void OnGameplayGoalAbandoned_Implementation(const FName& AbandonedGoalName) override;

public:
	// For Console Commands

	// Unlock the given recipe for current player.
	UFUNCTION(Exec, Category = "ConsoleCommands")
		void UnlockRecipe(const FString& RecipeName);

	// Unlock the given crop for current player.
	UFUNCTION(Exec, Category = "ConsoleCommands")
		void UnlockCrop(const FString& CropName);

	// Add GoodsName Quantity to current player inventory
	UFUNCTION(Exec, Category = "ConsoleCommands")
		void GiveGoods(const FString& GoodsName, const int32 Quantity = 1);

	// Harvest and add GoodsName Quantity to current player inventory
	UFUNCTION(Exec, Category = "ConsoleCommands")
		void HarvestGoods(const FString& GoodsName, const int32 Quantity = 1);

	// Add experience to current player
	UFUNCTION(Exec, Category = "ConsoleCommands")
		void GiveExperience(const int32 Experience);

	UFUNCTION(Exec, Category = "ConsoleCommands")
		void Award(const FString& AwardCommand);

	// Reload goal data from file.
	UFUNCTION(Exec, Category = "ConsoleCommands")
		void ReloadGoalData();

	// So we can pass the call up to BP. (instead of re-implementing components in c++ right now)
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ConsoleCommands")
		void GiveGoodsBP(const FString& GoodsName, const int32 Quantity = 1);

	// So we can pass the call up to BP. (instead of re-implementing components in c++ right now)
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ConsoleCommands")
		void HarvestGoodsBP(const FString& GoodsName, const int32 Quantity = 1);

	// So we can pass the call up to BP. (instead of re-implementing in c++ right now)
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ConsoleCommands")
		void AwardBP(const FString& AwardCommand);
};
