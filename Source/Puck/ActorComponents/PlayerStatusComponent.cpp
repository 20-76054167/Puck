// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatusComponent.h"

#include "PuckSlayer.h"
#include "Puck/Widgets/HUDUserWidget.h"


// Sets default values for this component's properties
UPlayerStatusComponent::UPlayerStatusComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	CurrentHealth = 100.0f;
	MaxHealth = 100.0f;

	
	// ...
}


// Called when the game starts
void UPlayerStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
	// ...
	
}


// Called every frame
void UPlayerStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlayerStatusComponent::TakeDamage(const float Amount, const FVector EnemyLocation)
{
	CurrentHealth -= Amount;
	OnDamageTaken.Broadcast();
	OnDamageTakenWithLocation.Broadcast(EnemyLocation);
	if (CurrentHealth <= 0.0f)
	{
		OnDeath.Broadcast();
	}
	Cast<APuckSlayer>(Owner)->HUD->SetHealthPercentage(CurrentHealth / MaxHealth);
}

void UPlayerStatusComponent::RecoveryHealth(const float Amount)
{
	CurrentHealth += Amount;
	if (CurrentHealth >= MaxHealth)
	{
		CurrentHealth = MaxHealth;
	}
	Cast<APuckSlayer>(Owner)->HUD->SetHealthPercentage(CurrentHealth / MaxHealth);
}

