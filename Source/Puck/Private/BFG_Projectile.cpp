// Fill out your copyright notice in the Description page of Project Settings.


#include "BFG_Projectile.h"
#include "PuckSlayer.h"
#include "PuckWeaponComponent.h"

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

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(CollisionComp);
	
	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
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
}

void ABFG_Projectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		FVector CurrentVelocity = GetVelocity();
		FVector Impulse = CurrentVelocity * ImpulseMultiplier;

		//Impulse.Z = FMath::Clamp(Impulse.Z, -1000000.0f, 0.0f);
		OtherComp->AddImpulseAtLocation(Impulse, GetActorLocation());
		Destroy();
	}
}

void ABFG_Projectile::SetDamage(float DamageAmount)
{
	Damage = DamageAmount;
	CalculateVelocity();
	UE_LOG(LogTemp, Warning, TEXT("Damage : %f"), Damage);
}

void ABFG_Projectile::CalculateVelocity()
{
	FVector ForwardVector = GetActorForwardVector();
	float CalculatedSpeed = Damage * 100 + 300;
	if(CalculatedSpeed < 0.0f)
	{
		CalculatedSpeed = 0.0f;
	}
	FVector Velocity = ForwardVector * CalculatedSpeed;
	//ProjectileMovement->SetVelocityInLocalSpace(Velocity);

	ProjectileMovement->Velocity = Velocity;
	ProjectileMovement->InitialSpeed = CalculatedSpeed;
	ProjectileMovement->MaxSpeed = CalculatedSpeed;

	UE_LOG(LogTemp, Warning, TEXT("CalculatedSpeed: %f, Velocity: %s"), CalculatedSpeed, *Velocity.ToString());
}

// Called every frame
void ABFG_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}