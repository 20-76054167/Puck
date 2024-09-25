// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "PuckWeaponComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PUCK_API UPuckWeaponComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()
public:
	UPuckWeaponComponent();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= "Projectile")
	TSubclassOf<class ABFG_Projectile> BFG_ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Sound")
	USoundBase* BFG_FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim Montage")
	UAnimMontage* BFG_FireAnimation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Gameplay")
	FVector MuzzleOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* FireMappingContext;

	/** Fire Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* FireAction;
	
	UFUNCTION(BlueprintCallable, Category="Weapon")
	bool AttachWeapon(class APuckSlayer* TargetCharacter);

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Fire();

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Projectile Damage")
	float BFG_Damage;
	
private:
	UPROPERTY()
	APuckSlayer* Character;

protected:
	/** Ends gameplay for this component. */
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void BeginPlay() override;
};
