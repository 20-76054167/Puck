// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

UCLASS()
class PUCK_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

	UPROPERTY(EditAnywhere, Category="WeaponOption")
	class USkeletalMeshComponent* SMesh;

	UPROPERTY(EditAnywhere, Category="WeaponOption")
	UDataTable* DtWeapon;

	UPROPERTY(EditAnywhere, Category="WeaponOption")
	float coolTime;

	UPROPERTY(EditAnywhere, Category="WeaponOption")
	float damage;

	UPROPERTY(EditAnywhere, Category="WeaponOption")
	float range;
	
	UPROPERTY(EditAnywhere, Category="WeaponOption")
	float recoil;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
