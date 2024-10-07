// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PuckGameMode.generated.h"

UCLASS(minimalapi)
class APuckGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APuckGameMode();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Music")
	USoundBase* BackGroundMusic_1;
};