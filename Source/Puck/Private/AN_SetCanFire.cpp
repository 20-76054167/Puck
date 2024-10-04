// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_SetCanFire.h"

#include "FireActorComponent.h"
#include "PuckSlayer.h"

void UAN_SetCanFire::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                            const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if(MeshComp->GetOwner())
	{
		APuckSlayer* PuckSlayer = Cast<APuckSlayer>(MeshComp->GetOwner());
		if(PuckSlayer)
		{
			PuckSlayer->fireActorComp->bCanAttack = true;
		}
	}
}
