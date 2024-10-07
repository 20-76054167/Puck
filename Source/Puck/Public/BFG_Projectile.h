// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BFG_Projectile.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;

UCLASS()
class PUCK_API ABFG_Projectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABFG_Projectile();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	UStaticMeshComponent* MeshComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects")
	class UNiagaraComponent* BulletNiagaraComp;

	// Niagara System을 로드하기 위한 변수
	UPROPERTY(EditAnywhere, Category = "Effects")
	class UNiagaraSystem* BulletNiagaraEffect;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UNiagaraSystem* HitNiagaraEffect;

	void SpawnNiagaraEffectAtLocation(UObject* WorldContextObject, UNiagaraSystem* NiagaraSystem, FVector Location, FRotator Rotation, FVector Scale);
	
	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void SetDamage(float DamageAmount);
	
	void CalculateVelocity();
	
	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; }
	
	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	//
	UPROPERTY()
	float Damage;
	
	float VelocityMultiplier = 150.0f;
	float VelocityMinimum = 300.0f;
	float ImpulseMultiplier = 700.0f;

private:
	FTimerHandle TimerHandle;

	// 타이머에 의해 호출될 함수
	void DelayDestroy();
};
