// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PuckSlayer.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "EWType.h"
#include "FireActorComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class PUCK_API UFireActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFireActorComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWType currentMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FireArrow")
	class UArrowComponent* fireArrow;

	UPROPERTY(EditAnywhere, Category="bullet option")
	float damage = 10;

	UPROPERTY(EditAnywhere, Category="bullet option")
	float range = 1000;

	UPROPERTY(EditAnywhere, Category="bullet option")
	float pitchCongestion = 200;

	UPROPERTY(EditAnywhere, Category="bullet option")
	float rollCongestion = 200;

	UPROPERTY(EditAnywhere, Category="bullet option")
	float yawCongestion = 200;
	
	UPROPERTY(EditAnywhere, Category="bullet option")
	int32 bulletNum = 6;
	
	UPROPERTY(EditAnywhere, Category="bullet option")
	int32 maxMagazineShotGun = 5;

	UPROPERTY(EditAnywhere, Category="bullet option")
	int32 magazineShotGun = 2;

	UPROPERTY(EditAnywhere, Category="bullet option")
	int32 maxMagazineRifle = 10;

	UPROPERTY(EditAnywhere, Category="bullet option")
	int32 magazineRifle = 6;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanAttack = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsAiming = false;

	APuckSlayer* ownerPlayer;
	APlayerController* playerController;
	
	UCurveFloat* recoilCurve;
	class UTimelineComponent* recoilTimeline;
	FOnTimelineFloat recoilStartCallback;
	FOnTimelineEvent recoilEndCallback;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ChangeActorMode(EWType currentMode);
	void SetRecoilTimeline();
	
	void FireByTrace();
	void Reload();

	void SetMagazine(int32 magazine);
	int32 GetCurrentMagazine();

	UFUNCTION()
	void RecoilStart(float value);
	UFUNCTION()
	void RecoveryRecoil();
};
