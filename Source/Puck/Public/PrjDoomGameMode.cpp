// Copyright Epic Games, Inc. All Rights Reserved.

#include "PrjDoomGameMode.h"
#include "PrjDoomCharacter.h"
#include "UObject/ConstructorHelpers.h"

APrjDoomGameMode::APrjDoomGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
