// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PuckSlayer.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "FireActorComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class PUCK_API UFireActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFireActorComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FireArrow")
	class UArrowComponent* fireArrow;

	UPROPERTY(EditAnywhere, Category="bullet option")
	float damage = 10;

	UPROPERTY(EditAnywhere, Category="bullet option")
	float range = 1000;

	UPROPERTY(EditAnywhere, Category="bullet option")
	float pitchRange;

	UPROPERTY(EditAnywhere, Category="bullet option")
	float rollRange;

	UPROPERTY(EditAnywhere, Category="bullet option")
	float yawRange;
	
	UPROPERTY(EditAnywhere, Category="bullet option")
	int32 bulletNum = 6;
	
	UPROPERTY(EditAnywhere, Category="bullet option")
	int32 maxMagazine = 2;

	UPROPERTY(EditAnywhere, Category="bullet option")
	int32 magazine;

	float attackDelay;
	
	bool bCanAttack = true;

	APuckSlayer* ownerPlayer;
	APlayerController* playerController;
	
	UCurveFloat* recoilCurve;
	class UTimelineComponent* recoilTimeline;
	FOnTimelineFloat recoilStartCallback;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void FireByTrace();
	
	bool Reload();

	void SetRecoilTimeline();

	UFUNCTION()
	void RecoilStart(float value);
	
	void RecoveryRecoil();
};
