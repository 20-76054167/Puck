// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDUserWidget.h"

#include "FireActorComponent.h"
#include "PuckSlayer.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UHUDUserWidget::SetHealthPercentage(const float Percent)
{
	if (HealthProgressBar) HealthProgressBar->SetPercent(Percent);
}

void UHUDUserWidget::SetWeaponIcon(UTexture2D* Icon)
{
	WeaponIcon->SetBrushFromTexture(Icon, true);
}

void UHUDUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PuckSlayer = Cast<APuckSlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	
}

void UHUDUserWidget::SetMagazine()
{
	const EWType CurrentWeaponType = PuckSlayer->CurrentEwType;
	int32 CurrentValue = 0;
	int32 MaxValue = 0;

	switch (CurrentWeaponType)
	{
	case EWType::Rifle:
		CurrentValue = PuckSlayer->fireActorComp->magazineRifle;
		MaxValue = PuckSlayer->fireActorComp->maxMagazineRifle;
		break;
	case EWType::Shotgun:
		CurrentValue = PuckSlayer->fireActorComp->magazineShotGun;
		MaxValue = PuckSlayer->fireActorComp->maxMagazineShotGun;
		break;
	}

	SetCurrentMagazine(CurrentValue);
	SetMaxMagazine(MaxValue);
}

void UHUDUserWidget::SetCurrentMagazine(int32 Magazine)
{
	CurrentMagazine->SetText(FText::AsNumber(Magazine));
}

void UHUDUserWidget::SetMaxMagazine(int32 Magazine)
{
	MaxMagazine->SetText(FText::AsNumber(Magazine));
}
