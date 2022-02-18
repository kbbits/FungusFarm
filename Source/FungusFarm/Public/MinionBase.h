// Copyright 2022

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Misc/Guid.h"
#include "MinionBase.generated.h"

UCLASS()
class FUNGUSFARM_API AMinionBase : public AActor
{
	GENERATED_BODY()

	/** Root component */
	//UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	//class USceneComponent* SceneRoot;

protected:

	// Guid of the equipment that spawned this minion. 
	UPROPERTY(BlueprintReadWrite, SaveGame)
		FGuid ParentSpawnerID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, SaveGame)
		float MaxSpeed = 100.0;

	// Mod equipment that spawned this minion.
	//UPROPERTY(BlueprintReadWrite)
	//AActor* ParentSpawner;


public:
	// Sets default values for this actor's properties
	AMinionBase();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
