// Fill out your copyright notice in the Description page of Project Settings.


#include "FireActorComponent.h"

#include <string>

#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UFireActorComponent::UFireActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	if(GetOwner())
	{
		ownerPlayer = Cast<APuckSlayer>(GetOwner());
	}

	ConstructorHelpers::FObjectFinder<UCurveFloat> curve(TEXT("/Script/Engine.CurveFloat'/Game/FireRecoilCurve.FireRecoilCurve'"));

	if(curve.Succeeded())
	{
		recoilCurve = curve.Object;
	}

	recoilTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline Component"));
	recoilStartCallback.BindUFunction(this, FName("RecoilStart"));
	recoilEndCallback.BindUFunction(this, FName("RecoveryRecoil"));
}


void UFireActorComponent::SetRecoilTimeline()
{
	if(recoilCurve)
	{
		recoilTimeline->AddInterpFloat(recoilCurve, recoilStartCallback);
		recoilTimeline->SetTimelineFinishedFunc(recoilEndCallback);
		recoilTimeline->SetLooping(false);
	}
}

// Called when the game starts
void UFireActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ..
	SetRecoilTimeline();
	
	AController* ownerController = ownerPlayer->GetController();
	if(ownerController)
	{
		playerController = Cast<APlayerController>(ownerController);
		
		if(ownerPlayer)
		{
			TArray<UArrowComponent*> ArrowComponents;
			ownerPlayer->GetComponents<UArrowComponent>(ArrowComponents);

			for(UArrowComponent* singleArrow : ArrowComponents)
			{
				if(singleArrow->GetName() == "CameraVector")
				{
					fireArrow = singleArrow;
				}
			}
		}
	}
}

// Called every frame
void UFireActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UFireActorComponent::ChangeActorMode(EWType changeMode)
{
	currentMode = changeMode;
}

void UFireActorComponent::FireByTrace()
{
	if(!bCanAttack)
	{
		return;
	}
	
	FVector _startLoc = fireArrow->GetComponentLocation();
	FVector _endLoc = _startLoc + fireArrow->GetForwardVector() * range;
	
	if(currentMode == EWType::Shotgun)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("shotgun fire"));
		for(int i=0; i < bulletNum; i++)
		{
			FHitResult _hitRes;

			FCollisionQueryParams _collisionParam;
			_collisionParam.AddIgnoredActor(ownerPlayer);

			_endLoc.X += FMath::RandRange(pitchCongestion * -1, pitchCongestion);
			_endLoc.Y += FMath::RandRange(rollCongestion * -1, rollCongestion);
			_endLoc.Z += FMath::RandRange(yawCongestion * -1, yawCongestion);
		
			bool isHit = GetWorld()->LineTraceSingleByChannel(_hitRes, _startLoc, _endLoc, ECC_Pawn, _collisionParam);
			DrawDebugLine(GetWorld(), _startLoc, _endLoc, FColor::Green, true, 5.f);
		
			if(isHit)
			{
				AActor* hitActor = _hitRes.GetActor();
				if(hitActor)
				{
					UGameplayStatics::ApplyDamage(hitActor, damage, ownerPlayer->GetController(), ownerPlayer, UDamageType::StaticClass());
				}
			}
		}
	
		recoilTimeline->PlayFromStart();
	}
	else if(currentMode == EWType::Rifle)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("Rifle fire"));
		FHitResult _hitRes;

		FCollisionQueryParams _collisionParam;
		_collisionParam.AddIgnoredActor(ownerPlayer);
		
		bool isHit = GetWorld()->LineTraceSingleByChannel(_hitRes, _startLoc, _endLoc, ECC_Pawn, _collisionParam);
		DrawDebugLine(GetWorld(), _startLoc, _endLoc, FColor::Green, true, 5.f);

		if(IsValid(playerController))
		{
			float yawRandom = FMath::RandRange(-1, 1);
			playerController->AddYawInput(yawRandom);
			playerController->AddPitchInput(-0.5);
		}
		
		if(isHit)
		{
			AActor* hitActor = _hitRes.GetActor();
			if(hitActor)
			{
				UGameplayStatics::ApplyDamage(hitActor, damage, ownerPlayer->GetController(), ownerPlayer, UDamageType::StaticClass());
			}
		}
	}
	
}

void UFireActorComponent::Reload()
{
	if(currentMode == EWType::Shotgun)
	{
		if(magazineShotGun >= maxMagazineShotGun)
		{
			return;
		}
		magazineShotGun++;
	}
	else if(currentMode == EWType::Rifle)
	{
		if(magazineRifle >= maxMagazineRifle)
		{
			return;
		}
		magazineRifle++;
	}
}

void UFireActorComponent::SetMagazine(int32 plusMagazine)
{
	if(currentMode == EWType::Shotgun)
	{
		if(magazineShotGun+plusMagazine < maxMagazineShotGun)
		{
			magazineShotGun = plusMagazine;
		}
		else
		{
			magazineShotGun = maxMagazineShotGun;
		}
	}
	else if(currentMode == EWType::Rifle)
	{
		if(magazineRifle+plusMagazine < maxMagazineRifle)
		{
			magazineRifle = plusMagazine;
		}
		else
		{
			magazineRifle = maxMagazineRifle;
		}
	}
}

int32 UFireActorComponent::GetCurrentMagazine()
{
	if(currentMode == EWType::Shotgun)
	{
		return magazineShotGun;
	}
	else if(currentMode == EWType::Rifle)
	{
		return magazineRifle;
	}
	else
	{
		return 1;
	}
}

void UFireActorComponent::RecoilStart(float value)
{
	if(IsValid(playerController))
	{
		playerController->AddPitchInput(value);
	}
}

void UFireActorComponent::RecoveryRecoil()
{
	if(IsValid(playerController))
	{
		playerController->AddPitchInput(3);
	}
}
