// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_Reload.h"

#include "FireActorComponent.h"
#include "Puck/Widgets/HUDUserWidget.h"

void UANS_Reload::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	//Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	if(MeshComp->GetOwner())
	{
		Player = Cast<APuckSlayer>(MeshComp->GetOwner());
	}
}

void UANS_Reload::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	//Super::NotifyEnd(MeshComp, Animation);

	if(Player && Player->fireActorComp)
	{
		Player->fireActorComp->Reload();
		Player->fireActorComp->bCanAttack = true;
		
		// set HUD Magazine Value
		Player->HUD->SetMagazine();
		
		if(Player->fireActorComp->IsFullMagazine())
		{
			UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
			if(AnimInstance)
			{
				if(AnimInstance->Montage_IsPlaying(Player->ReloadShotgunAnim))
				{
					AnimInstance->Montage_SetPosition(Player->ReloadShotgunAnim, 3.76);
				}
			}
		}
	}
}
