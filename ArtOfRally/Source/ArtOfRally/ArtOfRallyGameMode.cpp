// Copyright Epic Games, Inc. All Rights Reserved.

#include "ArtOfRallyGameMode.h"
#include "ArtOfRallyPlayerController.h"

AArtOfRallyGameMode::AArtOfRallyGameMode()
{
	PlayerControllerClass = AArtOfRallyPlayerController::StaticClass();
}

void AArtOfRallyGameMode::RemoveEnemy(AAOREnemy* EnemyToRemove)
{
	if (Enemies.Contains(EnemyToRemove))
	{
		Enemies.Remove(EnemyToRemove);
		OnEnemyRemoved(EnemyToRemove);
	}
}

void AArtOfRallyGameMode::AddEnemy(AAOREnemy* EnemyToAdd)
{
	if (!Enemies.Contains(EnemyToAdd))
	{
		Enemies.Add(EnemyToAdd);
	}
}
