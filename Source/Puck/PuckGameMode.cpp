// Copyright Epic Games, Inc. All Rights Reserved.

#include "PuckGameMode.h"
#include "PuckCharacter.h"
#include "UObject/ConstructorHelpers.h"

APuckGameMode::APuckGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
