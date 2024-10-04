// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PuckSlayer.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_Reload.generated.h"

/**
 * 
 */
UCLASS()
class PUCK_API UANS_Reload : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	APuckSlayer* Player;
	
	void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
