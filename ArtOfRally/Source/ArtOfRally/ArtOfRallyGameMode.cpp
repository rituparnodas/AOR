// Copyright Epic Games, Inc. All Rights Reserved.

#include "ArtOfRallyGameMode.h"
#include "ArtOfRallyPlayerController.h"

AArtOfRallyGameMode::AArtOfRallyGameMode()
{
	PlayerControllerClass = AArtOfRallyPlayerController::StaticClass();
}
