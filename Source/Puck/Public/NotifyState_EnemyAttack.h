// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "NormalEnemy.h"
#include "NotifyState_EnemyAttack.generated.h"

/**
 * 
 */
UCLASS()
class PUCK_API UNotifyState_EnemyAttack : public UAnimNotifyState
{
	GENERATED_BODY()
	

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	

	UPROPERTY(EditAnywhere, Category = "Enemy")
	ANormalEnemy* normalEnemy;

	
};
