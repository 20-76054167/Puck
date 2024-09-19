// Fill out your copyright notice in the Description page of Project Settings.


#include "PBullet.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
APBullet::APBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereCollComp = CreateDefaultSubobject<USphereComponent>(TEXT("MainCollidor"));
	SphereCollComp->SetCollisionProfileName(TEXT("BlockAll"));
	SphereCollComp->SetSphereRadius(13);
	RootComponent = SphereCollComp;

	SMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SMeshComp->SetupAttachment(SphereCollComp);
	SMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	PMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("PMovementComp"));
	PMovementComp->SetUpdatedComponent(SphereCollComp);
	PMovementComp->InitialSpeed = 5000.0f;
	PMovementComp->MaxSpeed = 8000.0f;
	PMovementComp->bShouldBounce = true;
	PMovementComp->Bounciness = 0.3f;
	InitialLifeSpan = 2.0f;
}

// Called when the game starts or when spawned
void APBullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

