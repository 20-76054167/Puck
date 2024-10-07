// Fill out your copyright notice in the Description page of Project Settings.


#include "FireActorComponent.h"

#include <string>

#include "CS_RifleRecoil.h"
#include "CS_ShotgunRecoil.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NormalEnemy.h"
#include "Particles/ParticleSystem.h"

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
	
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonMurdock/FX/Particles/Abilities/Primary/FX/P_PlasmaShot_Hit_World.P_PlasmaShot_Hit_World'")); // ���� ��ƼŬ ��η� ����

	if (ParticleAsset.Succeeded())
	{
		particleEffect = ParticleAsset.Object; // ��ƼŬ ����Ʈ�� ������ �Ҵ�
	}
}

// Called when the game starts
void UFireActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ..
	
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
				if(singleArrow->GetName() == "FireArrowComp")
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
		for(int i=0; i < bulletNum; i++)
		{
			FHitResult _hitRes;

			FCollisionQueryParams _collisionParam;
			_collisionParam.AddIgnoredActor(ownerPlayer);

			if(bIsAiming)
			{
				_endLoc.X += FMath::RandRange((pitchCongestion * 0.5) * -1, (pitchCongestion * 0.5));
				_endLoc.Y += FMath::RandRange((rollCongestion * 0.5) * -1, (rollCongestion * 0.5));
				_endLoc.Z += FMath::RandRange((yawCongestion * 0.5) * -1, (yawCongestion * 0.5));	
			}
			else
			{
				_endLoc.X += FMath::RandRange(pitchCongestion * -1, pitchCongestion);
				_endLoc.Y += FMath::RandRange(rollCongestion * -1, rollCongestion);
				_endLoc.Z += FMath::RandRange(yawCongestion * -1, yawCongestion);
			}
		
			bool isHit = GetWorld()->LineTraceSingleByChannel(_hitRes, _startLoc, _endLoc, ECC_Pawn, _collisionParam);
			DrawDebugLine(GetWorld(), _startLoc, _endLoc, FColor::Green, true, 5.f);
		
			if(isHit)
			{
				AActor* hitActor = _hitRes.GetActor();
				if(hitActor)
				{
					UGameplayStatics::ApplyDamage(hitActor, damage, ownerPlayer->GetController(), ownerPlayer, UDamageType::StaticClass());

					//FVector actorLocation = hitActor->GetActorLocation();
					FVector actorLocation = _hitRes.Location;

					
					if (particleEffect)
					{
						FVector newShootScale(4.0f, 4.0f, 4.0f);
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), particleEffect, actorLocation, FRotator::ZeroRotator, newShootScale, true);
					}
				}
			}
		}

		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(UCS_ShotgunRecoil::StaticClass());
	}
	else if(currentMode == EWType::Rifle)
	{
		FHitResult _hitRes;

		FCollisionQueryParams _collisionParam;
		_collisionParam.AddIgnoredActor(ownerPlayer);

		float yawRandom = FMath::RandRange(-0.3, 0.3);
		float pitchRandom = FMath::RandRange(-0.2, 0.1);

		if(bIsAiming)
		{
			_endLoc.X += pitchRandom;
			_endLoc.Z += yawRandom;
		}
		else
		{
			playerController->AddYawInput(yawRandom);
			playerController->AddPitchInput(pitchRandom);
		}
		
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(URecoilCameraShake::StaticClass());
		
		bool isHit = GetWorld()->LineTraceSingleByChannel(_hitRes, _startLoc, _endLoc, ECC_Pawn, _collisionParam);
		DrawDebugLine(GetWorld(), _startLoc, _endLoc, FColor::Green, true, 5.f);
		
		if(isHit)
		{
			AActor* hitActor = _hitRes.GetActor();
			if(hitActor)
			{
				UGameplayStatics::ApplyDamage(hitActor, damage, ownerPlayer->GetController(), ownerPlayer, UDamageType::StaticClass());

				//FVector actorLocation = hitActor->GetActorLocation();
				FVector actorLocation = _hitRes.Location;


				if (particleEffect)
				{
					FVector newShootScale(1.3f, 1.3f, 1.3f);
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), particleEffect, actorLocation, FRotator::ZeroRotator, newShootScale, true);
				}
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
		magazineRifle = maxMagazineRifle;
	}
}

void UFireActorComponent::SetMagazine(int32 newMagazine)
{
	if(currentMode == EWType::Shotgun)
	{
		if(newMagazine < maxMagazineShotGun)
		{
			magazineShotGun = newMagazine;
		}
		else
		{
			magazineShotGun = maxMagazineShotGun;
		}
	}
	else if(currentMode == EWType::Rifle)
	{
		if(newMagazine < maxMagazineRifle)
		{
			magazineRifle = newMagazine;
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

bool UFireActorComponent::IsFullMagazine()
{
	int maxMagazine = 0;
	if(currentMode == EWType::Shotgun)
	{
		maxMagazine = maxMagazineShotGun;
	}
	else if(currentMode == EWType::Rifle)
	{
		maxMagazine = maxMagazineRifle;
	}

	if(GetCurrentMagazine() >= maxMagazine)
	{
		return true;
	}
	else
	{
		return false;
	}
}
