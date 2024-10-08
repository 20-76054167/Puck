// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_Fire.h"

#include "FireActorComponent.h"
#include "PuckSlayer.h"
#include "Puck/Widgets/HUDUserWidget.h"

void UAN_Fire::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                      const FAnimNotifyEventReference& EventReference)
{
	//Super::Notify(MeshComp, Animation, EventReference);
	
	if(MeshComp->GetOwner())
	{
		APuckSlayer* PuckSlayer = Cast<APuckSlayer>(MeshComp->GetOwner());
		if(PuckSlayer)
		{
			int32 currnetMagazine = PuckSlayer->fireActorComp->GetCurrentMagazine(); 
			PuckSlayer->fireActorComp->FireByTrace();
			PuckSlayer->fireActorComp->bCanAttack = false;
			currnetMagazine--;
			PuckSlayer->fireActorComp->SetMagazine(currnetMagazine);

			// set HUD Magazine Value
			if(PuckSlayer->HUD)
			{
				PuckSlayer->HUD->SetMagazine();
			}
		}
	}
}
