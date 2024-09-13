// Fill out your copyright notice in the Description page of Project Settings.


#include "BFG_Projectile.h"

// Sets default values
ABFG_Projectile::ABFG_Projectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABFG_Projectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABFG_Projectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
}

void ABFG_Projectile::SetDamage(float DamageAmount)
{
}

void ABFG_Projectile::CalculateVelocity()
{
}

// Called every frame
void ABFG_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}