// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EliteEnemy.generated.h"

UCLASS()
class PUCK_API AEliteEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEliteEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NormalEnemy Status")
	float EliteEnemyHealth;

	UPROPERTY(EditAnywhere, Category = "NormalEnemy Status")
	float EliteDamageAmount = 10.0f;

	UFUNCTION(BlueprintCallable, Category = "EnemyChaseAndAttack")
	void AttackPlayer();

	float TakeDamage(float takenDamage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:

	void Die();
	UParticleSystem* EDeathEffect; // 파티클 이펙트 변수
};
