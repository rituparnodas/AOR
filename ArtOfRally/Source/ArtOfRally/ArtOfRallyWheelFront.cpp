// Copyright Epic Games, Inc. All Rights Reserved.

#include "ArtOfRallyWheelFront.h"
#include "UObject/ConstructorHelpers.h"

UArtOfRallyWheelFront::UArtOfRallyWheelFront()
{
	AxleType = EAxleType::Front;
	bAffectedBySteering = true;
	MaxSteerAngle = 40.f;
}