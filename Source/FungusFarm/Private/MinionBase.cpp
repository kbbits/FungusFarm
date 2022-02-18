// Copyright 2022


#include "MinionBase.h"


// Default constructor
AMinionBase::AMinionBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create root scene component
	//SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	//RootComponent = SceneRoot;
}


// Called when the game starts or when spawned
void AMinionBase::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void AMinionBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

