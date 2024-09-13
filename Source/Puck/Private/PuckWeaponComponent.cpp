// Fill out your copyright notice in the Description page of Project Settings.


#include "PuckWeaponComponent.h"
#include "BFG_Projectile.h"
#include "PuckSlayer.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"

UPuckWeaponComponent::UPuckWeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
	PrimaryComponentTick.bCanEverTick = true;
	static ConstructorHelpers::FClassFinder<ABFG_Projectile>ProjectileBPClass(TEXT("/Game/Blueprints/BP_BFG_Projectile.BP_BFG_Projectile"));
	BFG_ProjectileClass = ProjectileBPClass.Class;

	BFG_Damage = 100.f;
}

bool UPuckWeaponComponent::AttachWeapon(class APuckSlayer* TargetCharacter)
{
	Character = TargetCharacter;

	// Check that the character is valid, and has no weapon component yet
	if (Character == nullptr || Character->GetInstanceComponents().FindItemByClass<UPuckWeaponComponent>())
	{
		return false;
	}

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh(), AttachmentRules, FName(TEXT("TempFireLoc")));

	// add the weapon as an instance component to the character
	Character->AddInstanceComponent(this);
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &UPuckWeaponComponent::Fire);
		}
	}
	return true;
}

void UPuckWeaponComponent::Fire()
{
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}
	// Try and fire a projectile
	if (BFG_ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position

			//bug temp
			const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// Try and play the sound if specified
			if (BFG_FireSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, BFG_FireSound, Character->GetActorLocation());
			}
			// Try and play a firing animation if specified
			if (BFG_FireAnimation != nullptr)
			{
				// Get the animation object for the arms mesh
				UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
				if (AnimInstance != nullptr)
				{
					AnimInstance->Montage_Play(BFG_FireAnimation, 1.f);
				}
			}
			// Spawn the projectile at the muzzle
			ABFG_Projectile* Projectile = World->SpawnActor<ABFG_Projectile>(BFG_ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			if (Projectile)
			{
				Projectile->SetDamage(BFG_Damage);
				Projectile->ProjectileMovement->bRotationFollowsVelocity = true;
				Projectile->SetActorRotation(SpawnRotation);
			}
		}
	}
}

void UPuckWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Character == nullptr)
	{
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(FireMappingContext);
		}
	}
}
