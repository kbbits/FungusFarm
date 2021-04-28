// Fill out your copyright notice in the Description page of Project Settings.


#include "FFPlayerControllerBase.h"
#include "Engine/World.h"
#include "FFGameMode.h"

AFFPlayerControllerBase::AFFPlayerControllerBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GameplayGoalsManager = ObjectInitializer.CreateDefaultSubobject<UActorGoalsComponent>(this, TEXT("GameplayGoalsManager"));
}


void AFFPlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();

	// Add game mode's component as goal provider
	auto World = GetWorld();
	if (World)
	{
		AFFGameMode* MyGameMode = Cast<AFFGameMode>(World->GetAuthGameMode());
		if (MyGameMode)
		{
			if (MyGameMode->GameplayGoalsProvider)
			{
				GameplayGoalsManager->AddGoalProvider(MyGameMode->GameplayGoalsProvider);
			}
			else
			{
				UE_LOG(LogFFGame, Warning, TEXT("%s, no goals provider on game mode"), *GetNameSafe(this));
			}
		}
		else
		{
			UE_LOG(LogFFGame, Warning, TEXT("%s, game mode is not a valid FFGameMode"), *GetNameSafe(this));
		}
	}
	else
	{
		UE_LOG(LogFFGame, Warning, TEXT("%s, No game world"), *GetNameSafe(this));
	}
}


void AFFPlayerControllerBase::OnNewGameplayGoals_Implementation(const TArray<FGameplayGoal>& NewGoals)
{
	// Subclass provide meaningful implemetation

	return;
}


void AFFPlayerControllerBase::OnGameplayGoalsCompleted_Implementation(const TArray<FGameplayGoal>& CompletedGoals)
{
	// Subclass provide meaningful implementation

	return;
}


void AFFPlayerControllerBase::OnGameplayGoalAbandoned_Implementation(const FName& AbandonedGoalName)
{
	// Subclass provide meaningful implementation

	return;
}

void AFFPlayerControllerBase::UnlockRecipe(const FString& RecipeName)
{
	AFFPlayerState* MyPlayerState = GetPlayerState<AFFPlayerState>();
	if (MyPlayerState)
	{
		MyPlayerState->UnlockRecipe(FName(*RecipeName));
	}
}

void AFFPlayerControllerBase::UnlockCrop(const FString& CropName)
{
	AFFPlayerState* MyPlayerState = GetPlayerState<AFFPlayerState>();
	if (MyPlayerState)
	{
		MyPlayerState->UnlockCrop(FName(*CropName));
	}
}

void AFFPlayerControllerBase::GiveGoods(const FString& GoodsName, const int32 Quantity)
{
	this->GiveGoodsBP(GoodsName, Quantity);
}

void AFFPlayerControllerBase::HarvestGoods(const FString& GoodsName, const int32 Quantity)
{
	this->HarvestGoodsBP(GoodsName, Quantity);
}

void AFFPlayerControllerBase::GiveExperience(const int32 Experience)
{
	AFFPlayerState* MyPlayerState = GetPlayerState<AFFPlayerState>();
	if (MyPlayerState)
	{
		MyPlayerState->AddExperience(Experience);
	}
}

void AFFPlayerControllerBase::Award(const FString & AwardsCommand)
{
	this->AwardBP(AwardsCommand);
}

void AFFPlayerControllerBase::ReloadGoalData()
{
	GameplayGoalsManager->ReloadGoalData();
}



