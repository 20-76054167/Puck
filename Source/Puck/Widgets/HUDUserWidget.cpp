// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDUserWidget.h"

#include "Components/Image.h"
#include "Components/ProgressBar.h"

void UHUDUserWidget::SetHealthPercentage(const float Percent)
{
	if (HealthProgressBar) HealthProgressBar->SetPercent(Percent);
}

void UHUDUserWidget::SetWeaponIcon(UTexture2D* Icon)
{
	WeaponIcon->SetBrushFromTexture(Icon, true);
}
