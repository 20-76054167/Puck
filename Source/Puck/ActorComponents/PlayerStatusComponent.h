// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStatusComponent.generated.h"

enum class EWType : uint8;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamageTakenWithLocationSignature, FVector, EnemyLocation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDamageTakenSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathSignature);
UCLASS(ClassGroup=(Custom),Blueprintable ,meta=(BlueprintSpawnableComponent))
class PUCK_API UPlayerStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerStatusComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Owner")
	AActor* Owner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth;

	UFUNCTION(BlueprintCallable, Category = "Health")
	void TakeDamage(float Amount, FVector EnemyLocation);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void RecoveryHealth(float Amount);

	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnDamageTakenSignature OnDamageTaken;

	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnDeathSignature OnDeath;

	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnDamageTakenWithLocationSignature OnDamageTakenWithLocation;
};
