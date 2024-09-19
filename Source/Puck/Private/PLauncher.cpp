// Fill out your copyright notice in the Description page of Project Settings.


#include "PLauncher.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
APLauncher::APLauncher()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	launcherComp = CreateDefaultSubobject<UBoxComponent>(TEXT("launcherComp"));
	RootComponent = launcherComp;
	launcherComp->SetBoxExtent(FVector(10, 50, 10));
	launcherComp->SetCollisionProfileName(TEXT("NoCollision"));


	launcherMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LauncherMesh"));
	launcherMesh->SetupAttachment(RootComponent);

	ConstructorHelpers::FObjectFinder<UStaticMesh> InitLauncher(TEXT("/Script/Engine.StaticMesh'/Game/Blueprints/PLauncher_StaticMesh.PLauncher_StaticMesh'"));
	if (InitLauncher.Succeeded())
	{
		launcherMesh->SetStaticMesh(InitLauncher.Object);
	}
}

// Called when the game starts or when spawned
void APLauncher::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APLauncher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

