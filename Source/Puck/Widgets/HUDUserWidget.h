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
	// magazine
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrentMagazine;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MaxMagazine;

	UFUNCTION(BlueprintCallable, category = "HUD")
	void SetHealthPercentage(float Percent);

	UFUNCTION(BlueprintCallable, category = "HUD")
	void SetWeaponIcon(UTexture2D* Icon);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "HUD")
	class APuckSlayer* PuckSlayer;

	UFUNCTION(BlueprintCallable, category = "HUD")
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, category = "Magazine")
	void SetMagazine();

	UFUNCTION(BlueprintCallable, category = "Magazine")
	void SetCurrentMagazine(int32 Magazine);

	UFUNCTION(BlueprintCallable, category = "Magazine")
	void SetMaxMagazine(int32 Magazine);
};
