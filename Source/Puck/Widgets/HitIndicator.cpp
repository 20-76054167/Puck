// Fill out your copyright notice in the Description page of Project Settings.


#include "HitIndicator.h"

#include "Components/Border.h"

void UHitIndicator::NativeConstruct()
{
	Super::NativeConstruct();

	MatInstance = HitIndicator_Border->GetDynamicMaterial();
}
