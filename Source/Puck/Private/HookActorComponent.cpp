// Fill out your copyright notice in the Description page of Project Settings.


#include "HookActorComponent.h"

#include "Components/ArrowComponent.h"

// Sets default values for this component's properties
UHookActorComponent::UHookActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	//HookMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static"));

	//HookArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT(""));
}


// Called when the game starts
void UHookActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHookActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

