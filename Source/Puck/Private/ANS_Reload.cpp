// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_Reload.h"

#include "FireActorComponent.h"

void UANS_Reload::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	if(MeshComp->GetOwner())
	{
		Player = Cast<APuckSlayer>(MeshComp->GetOwner());
	}
}

void UANS_Reload::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	if(Player && Player->fireActorComp)
	{
		Player->fireActorComp->Reload();
	}
}
