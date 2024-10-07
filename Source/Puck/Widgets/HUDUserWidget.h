// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDUserWidget.generated.h"

class UProgressBar;
/**
 * 
 */
UCLASS(Blueprintable)
class PUCK_API UHUDUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthProgressBar;

	UPROPERTY(meta = (BindWidget))
	class UImage* WeaponIcon;

	UFUNCTION(BlueprintCallable, category = "HUD")
	void SetHealthPercentage(float Percent);

	UFUNCTION(BlueprintCallable, category = "HUD")
	void SetWeaponIcon(UTexture2D* Icon);
};
