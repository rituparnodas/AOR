// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ArtOfRallyGameMode.generated.h"


class AAORPlayerState;
class AAOREnemy;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpgrade, FName, Option);

UCLASS(abstract)
class AArtOfRallyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AArtOfRallyGameMode();

	void RemoveEnemy(AAOREnemy* EnemyToRemove);

	UFUNCTION(BlueprintCallable)
	void AddEnemy(AAOREnemy* EnemyToAdd);

	UFUNCTION(BlueprintImplementableEvent)
	void OnEnemyRemoved(AAOREnemy* NewEnemy);

	UPROPERTY(BlueprintReadWrite)
	TArray<AAOREnemy*> Enemies;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Events")
	FOnUpgrade OnUpgrade;

};



