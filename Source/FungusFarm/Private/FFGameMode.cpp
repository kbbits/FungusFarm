// Fill out your copyright notice in the Description page of Project Settings.


#include "FFGameMode.h"


AFFGameMode::AFFGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GameplayGoalsProvider = ObjectInitializer.CreateDefaultSubobject<UGoalsProviderComponent>(this, TEXT("GoalsProvider"));
	UE_LOG(LogFFGame, VeryVerbose, TEXT("FFGameMode constructed Goals Provider %s"), (GameplayGoalsProvider == nullptr ? TEXT("INVALID") : TEXT("VALID")));
}

void AFFGameMode::BeginPlay()
{
	Super::BeginPlay();
		
}
