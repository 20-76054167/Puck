// Fill out your copyright notice in the Description page of Project Settings.


#include "BFG_Projectile.h"
#include "PuckSlayer.h"
#include "PuckWeaponComponent.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ABFG_Projectile::ABFG_Projectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ABFG_Projectile::OnHit);		// set up a notification for when this component hits something blocking
	// Set as root component
	RootComponent = CollisionComp;
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(CollisionComp);

	BulletNiagaraComp= CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	BulletNiagaraComp->SetupAttachment(CollisionComp);
	
	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;
	
	//Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = RootComponent;
	ProjectileMovement->InitialSpeed = 1000.f;
	ProjectileMovement->MaxSpeed = 1000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

// Called when the game starts or when spawned
void ABFG_Projectile::BeginPlay()
{
	Super::BeginPlay();
	CalculateVelocity();
	if (BulletNiagaraEffect)
	{
		BulletNiagaraComp->SetAsset(BulletNiagaraEffect);
		BulletNiagaraComp->Activate(true);
	}
}
// 특정 위치에 Niagara 이펙트를 스폰하는 함수
void ABFG_Projectile::SpawnNiagaraEffectAtLocation(UObject* WorldContextObject, UNiagaraSystem* NiagaraSystem, FVector Location, FRotator Rotation, FVector Scale)
{
	if (NiagaraSystem)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			WorldContextObject,  // 월드 컨텍스트 오브젝트
			NiagaraSystem,       // 스폰할 Niagara 시스템
			Location,            // 스폰 위치
			Rotation,            // 스폰 회전
			Scale,               // 스케일
			true,                // 자동으로 소멸할지 여부
			true,                // AttachToComponent를 사용할지 여부
			ENCPoolMethod::AutoRelease  // 풀링 방법
		);
	}
}

void ABFG_Projectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		FVector SpawnLocation = Hit.ImpactPoint;  // 이펙트를 스폰할 위치
		FRotator SpawnRotation = FRotator::ZeroRotator;  // 기본 회전
		FVector SpawnScale = FVector(1.0f);  // 기본 스케일

		// Niagara 이펙트 시스템을 특정 위치에 스폰
		SpawnNiagaraEffectAtLocation(GetWorld(), HitNiagaraEffect, SpawnLocation, SpawnRotation, SpawnScale);
		//GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ABFG_Projectile::DelayDestroy, 1.0f, false);
	}
}

void ABFG_Projectile::DelayDestroy()
{
	Destroy();
}

void ABFG_Projectile::SetDamage(float DamageAmount)
{
	Damage = DamageAmount;
	CalculateVelocity();
}

void ABFG_Projectile::CalculateVelocity()
{
	FVector ForwardVector = GetActorForwardVector();
	float CalculatedSpeed = Damage * 100;
	if(CalculatedSpeed < 0.0f)
	{
		CalculatedSpeed = 0.0f;
	}
	
	FVector Velocity = ForwardVector * CalculatedSpeed;
	ProjectileMovement->SetVelocityInLocalSpace(Velocity);
	ProjectileMovement->Velocity = Velocity;
}



// Called every frame
void ABFG_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}