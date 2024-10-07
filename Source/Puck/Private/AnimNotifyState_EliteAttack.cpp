// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_EliteAttack.h"

void UAnimNotifyState_EliteAttack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{

	eliteEnemy = Cast<AEliteEnemy>(MeshComp->GetOwner());

	if (eliteEnemy)
	{
		eliteEnemy->AttackPlayer();
	}
}

void UAnimNotifyState_EliteAttack::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
}

void UAnimNotifyState_EliteAttack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
}
