// Fill out your copyright notice in the Description page of Project Settings.


#include "SwapAnimNotifyState.h"

#include "PuckSlayer.h"
#include "PuckWeaponComponent.h"
#include "Kismet/GameplayStatics.h"

void USwapAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                       float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	PuckSlayer = Cast<APuckSlayer>(MeshComp->GetOwner());
	if (!IsValid(PuckSlayer))
	{
		UE_LOG(LogTemp, Error, TEXT("!IsValid(PuckSlayer)"))
		return;
	}

	UPuckWeaponComponent* EquippedWeapon;
	UPuckWeaponComponent* BackWeapon;
	
	if (PuckSlayer->Rifle->AttachedSocketName.IsEqual("WeaponSocket"))
	{
		EquippedWeapon = PuckSlayer->Rifle;
		BackWeapon = PuckSlayer->Shotgun;
	}
	else if (PuckSlayer->Rifle->AttachedSocketName.IsEqual("WeaponBackSocket"))
	{
		EquippedWeapon = PuckSlayer->Shotgun;
		BackWeapon = PuckSlayer->Rifle;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("!IsValid(PuckSlayer)"))
		return;
	}

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	EquippedWeapon->AttachToComponent(MeshComp, AttachmentRules, "WeaponBackSocket");
	EquippedWeapon->AttachedSocketName = "WeaponBackSocket";
	BackWeapon->AttachToComponent(MeshComp, AttachmentRules, "WeaponSocket");
	BackWeapon->AttachedSocketName = "WeaponSocket";
}

void USwapAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (PuckSlayer->CurrentEwType == EWType::Rifle)
	{
		PuckSlayer->CurrentEwType = EWType::Shotgun;
	}
	else
	{
		PuckSlayer->CurrentEwType = EWType::Rifle;
	}
}
