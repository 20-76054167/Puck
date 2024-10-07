// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HitIndicator.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class PUCK_API UHitIndicator : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	class UBorder* HitIndicator_Border;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitIndicator")
	class UMaterialInstanceDynamic* MatInstance;

	virtual void NativeConstruct() override;
};
