// Copyright 2020

#include "FungusFarm.h"
#include "HasModifiableAttributes.h"
#include "GlobalManager.h"

// Sets default values
AGlobalManager::AGlobalManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


// Called when the game starts or when spawned
void AGlobalManager::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void AGlobalManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AGlobalManager::GetGlobalEffects(TArray<FGlobalEffect>& GlobalEffects)
{
	CurrentGlobalEffects.GenerateValueArray(GlobalEffects);
};


void AGlobalManager::AddGlobalEffect(const FGlobalEffect& Effect)
{
	CurrentGlobalEffects.Add(Effect.UniqueName, Effect);
	RefreshEffectedObjects();
}


void AGlobalManager::AddGlobalEffects(const TArray<FGlobalEffect>& Effects)
{
	for (FGlobalEffect CurEffect : Effects)
	{
		CurrentGlobalEffects.Add(CurEffect.UniqueName, CurEffect);
	}
	RefreshEffectedObjects();
}


bool AGlobalManager::RemoveGlobalEffect(const FName EffectUniqueName)
{
	//FGlobalEffect RemovedEffect;
	if (CurrentGlobalEffects.Contains(EffectUniqueName))
	{
		//RemovedEffect = CurrentGlobalEffects.FindRef(EffectUniqueName);
		CurrentGlobalEffects.Remove(EffectUniqueName);
		RefreshEffectedObjects();
		return true;
	}
	return false;
}


bool AGlobalManager::RemoveGlobalEffects(const TArray<FName> EffectUniqueNames)
{
	int32 RemovedCount = 0;
	for (FName EffectName : EffectUniqueNames)
	{
		if (CurrentGlobalEffects.Contains(EffectName))
		{
			//RemovedEffect = CurrentGlobalEffects.FindRef(EffectUniqueName);
			CurrentGlobalEffects.Remove(EffectName);
			RemovedCount++;
		}
	}
	if (RemovedCount > 0)
	{
		RefreshEffectedObjects();
		return true;
	}
	return false;
}


void AGlobalManager::RefreshEffectedObjects_Implementation()
{
	TArray<AActor*> ModifiableActors;

	check(GetWorld());
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UHasModifiableAttributes::StaticClass(), ModifiableActors);
	for (AActor* ModActor : ModifiableActors)
	{
		IHasModifiableAttributes::Execute_RefreshAttributeModifierCalculations(ModActor);
	}
}