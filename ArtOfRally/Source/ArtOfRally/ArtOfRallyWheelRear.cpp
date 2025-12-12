// Copyright Epic Games, Inc. All Rights Reserved.

#include "ArtOfRallyWheelRear.h"
#include "UObject/ConstructorHelpers.h"

UArtOfRallyWheelRear::UArtOfRallyWheelRear()
{
	AxleType = EAxleType::Rear;
	bAffectedByHandbrake = true;
	bAffectedByEngine = true;
}