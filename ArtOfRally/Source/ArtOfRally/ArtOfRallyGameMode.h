// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ArtOfRallyGameMode.generated.h"


class AAORPlayerState;
class AAOREnemy;


UCLASS(abstract)
class AArtOfRallyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AArtOfRallyGameMode();
};



