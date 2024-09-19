// Fill out your copyright notice in the Description page of Project Settings.


#include "PuckPickUpComponent.h"

UPuckPickUpComponent::UPuckPickUpComponent()
{
	// Setup the Sphere Collision
	SphereRadius = 32.f;
}

void UPuckPickUpComponent::BeginPlay()
{
	Super::BeginPlay();

	// Register our Overlap Event
	OnComponentBeginOverlap.AddDynamic(this, &UPuckPickUpComponent::OnSphereBeginOverlap);
}

void UPuckPickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APuckSlayer* Character = Cast<APuckSlayer>(OtherActor);
	if(Character != nullptr)
	{
		// Notify that the actor is being picked up
		OnPickUp.Broadcast(Character);

		// Unregister from the Overlap Event so it is no longer triggered
		OnComponentBeginOverlap.RemoveAll(this);
	}
}