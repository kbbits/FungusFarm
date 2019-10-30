// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayGoal.h"
#include "ActorGoalsComponent.generated.h"


UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FUNGUSFARM_API UActorGoalsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActorGoalsComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TArray<FGameplayGoal> CurrentGoals;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
		TArray<FName> CompletedGoals;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	bool CheckGoalQualifiesComplete(const FGameplayGoal& Goal);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Gameplay Goals")
		void CheckForNewGoals();

	UFUNCTION(BlueprintCallable, Category = "Gameplay Goals")
		void CheckForCompletedGoals();

	UFUNCTION(BlueprintCallable, Category = "Gameplay Goals")
		void SetGoalComplete(const FGameplayGoal& Goal);

	UFUNCTION(BlueprintCallable, Category = "Gameplay Goals")
		void UpdateHarvestedGoodsProgress(const TArray<FGoodsQuantity>& HarvestedGoods);

	UFUNCTION(BlueprintCallable, Category = "Gameplay Goals")
		void UpdateSoldGoodsProgress(const TArray<FGoodsQuantity>& SoldGoods);

	UFUNCTION(BlueprintCallable, Category = "Gameplay Goals")
		void UpdateCraftedRecipesProgress(const FName& RecipeName, const int NumberCrafted);
		
};
