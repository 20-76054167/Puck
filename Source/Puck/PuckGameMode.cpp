// Copyright Epic Games, Inc. All Rights Reserved.

#include "PuckGameMode.h"
#include "PuckCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"

APuckGameMode::APuckGameMode()
{
	// set default pawn class to our Blueprinted character
	// static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_PuckSlayer.BP_PuckSlayer'"));
	// if (PlayerPawnBPClass.Class != NULL)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("APuckGameMode Class Initialized"));
	// 	DefaultPawnClass = PlayerPawnBPClass.Class;
	// }
}

void APuckGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (BackGroundMusic_1)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), BackGroundMusic_1);
	}
}
