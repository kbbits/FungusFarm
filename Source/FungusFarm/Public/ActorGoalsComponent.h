// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Delegates/Delegate.h"
#include "Components/ActorComponent.h"
#include "GameplayGoal.h"
#include "GameplayGoalProvider.h"
#include "ActorGoalsComponent.generated.h"

// Event dispatcher for when we hit minimum value
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGoalProgressChanged, const TArray<FGameplayGoal>&, ChangedGoals);

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FUNGUSFARM_API UActorGoalsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActorGoalsComponent();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay Goals")
		TArray<TScriptInterface<IGameplayGoalProvider>> GoalProviders;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "Gameplay Goals")
		TArray<FGameplayGoal> CurrentGoals;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "Gameplay Goals")
		TArray<FName> CompletedGoals;

	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
		FOnGoalProgressChanged OnProgressChanged;

	// If true, this component will automatically check for new goals when others are set as complete. (via SetGoalsComplete)
	// If this is false, other game code must call CheckForNewGoals() as needed.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "Gameplay Goals")
		bool bCheckForNewOnComplete;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Determine if this goal meets it's completion requirements.
	bool CheckGoalQualifiesComplete(const FGameplayGoal& Goal);

	void InitGoalProviders();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Gameplay Goals")
		void AddGoalProvider(const TScriptInterface<IGameplayGoalProvider>& NewProvider);

	UFUNCTION(BlueprintCallable, Category = "Gameplay Goals")
		int32 RemoveGoalProvider(const TScriptInterface<IGameplayGoalProvider>& ToRemoveProvider);

	UFUNCTION(BlueprintCallable, Category = "Gameplay Goals")
		const FString GetGoalProviderFriendlyName(const FGameplayGoal& Goal);

	UFUNCTION(BlueprintCallable, Category = "Gameplay Goals")
		void CheckForNewGoals();

	UFUNCTION(BlueprintCallable, Category = "Gameplay Goals")
		void CreateNewRandomGoal(const int MinTier, const int MaxTier, bool& bSuccess, FGameplayGoal& NewRandomGoal);

	UFUNCTION(BlueprintCallable, Category = "Gameplay Goals")
		void CheckForCompletedGoals();

	UFUNCTION(BlueprintCallable, Category = "Gameplay Goals")
		void SetGoalsComplete(const TArray<FGameplayGoal>& Goals, const bool bCheckForNewGoals);

	UFUNCTION(BlueprintCallable, Category = "Gameplay Goals")
		void UpdateHarvestedGoodsProgress(const TArray<FGoodsQuantity>& HarvestedGoods);

	UFUNCTION(BlueprintCallable, Category = "Gameplay Goals")
		void UpdateSoldGoodsProgress(const TArray<FGoodsQuantity>& SoldGoods);

	UFUNCTION(BlueprintCallable, Category = "Gameplay Goals")
		bool UpdateDonatedGoodsProgress(const TArray<FGoodsQuantity>& DonatedGoods, const FName GoalUniqueName);

	UFUNCTION(BlueprintCallable, Category = "Gameplay Goals")
		void UpdateCraftedRecipesProgress(const FName& RecipeName, const int NumberCrafted);

	UFUNCTION(BlueprintCallable, Category = "Gameplay Goals")
		bool GetCurrentGoalData(const FName GoalName, FGameplayGoal& GoalData);
		
};