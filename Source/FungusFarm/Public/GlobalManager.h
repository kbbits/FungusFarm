// Copyright 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GlobalEffect.h"
#include "GlobalManager.generated.h"

UCLASS()
class FUNGUSFARM_API AGlobalManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGlobalManager();

protected:

	// Internal map of active effects, mapped with effect.UniqueName as key.
	UPROPERTY(EditInstanceOnly)
		TMap<FName, FGlobalEffect> CurrentGlobalEffects;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure)
		void GetGlobalEffects(TArray<FGlobalEffect>& GlobalEffects);
		
	UFUNCTION(BlueprintCallable)
		void AddGlobalEffect(const FGlobalEffect& Effect);

	UFUNCTION(BlueprintCallable)
		void AddGlobalEffects(const TArray<FGlobalEffect>& Effects);

	// Removes an effect. Returns true if one was removed, false if no matching effect found.
	UFUNCTION(BlueprintCallable)
		bool RemoveGlobalEffect(const FName EffectUniqueName);

	// Removes all effects with matching name(s). Returns true if at least one was removed, false if no matching effect(s) found.
	UFUNCTION(BlueprintCallable)
		bool RemoveGlobalEffects(const TArray<FName> EffectUniqueNames);

	// Refresh all modifiable objects with the current global modifiers.
	// Should not need to call this manually. Add/Remove functions call this.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void RefreshEffectedObjects();
};
