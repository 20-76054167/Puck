// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "NormalEnemy.generated.h"

UCLASS()
class PUCK_API ANormalEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANormalEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;




	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NormalEnemy Status")
	float EnemyHealth;

	UPROPERTY(EditAnywhere, Category = "NormalEnemy Status")
	float DamageAmount = 5.0f;


	UFUNCTION(BlueprintCallable, Category ="EnemyChaseAndAttack")
	void EnemyFollowCharacter();

	UFUNCTION(BlueprintCallable, Category = "EnemyChaseAndAttack")
	void AttackPlayer();

	UPROPERTY(EditAnywhere, Category = "Animation");
	UAnimMontage* attackAnimMontage;


	float TakeDamage(float takenDamage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable)
	void Die();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Execution")
	bool bIsPendingForDeath = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SoundEffects")
	class USoundBase* PoofSound;
private:

	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* EparticleEffect; // ��ƼŬ ����Ʈ ����

	UParticleSystemComponent* particleComponent;

	UParticleSystem* EDeathEffect; // ��ƼŬ ����Ʈ ����
};
