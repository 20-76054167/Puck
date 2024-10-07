// Fill out your copyright notice in the Description page of Project Settings.


#include "EliteEnemy.h"

#include "PuckSlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Puck/ActorComponents/PlayerStatusComponent.h"

// Sets default values
AEliteEnemy::AEliteEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	EliteEnemyHealth = 50.f;
}

// Called when the game starts or when spawned
void AEliteEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEliteEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEliteEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEliteEnemy::AttackPlayer()
{

	FVector _Start = GetActorLocation();
	FVector _End = GetActorLocation() + GetActorForwardVector() * 300.0f;
	FHitResult _HitOut;

	FCollisionQueryParams _TraceParams;
	_TraceParams.AddIgnoredActor(this);
	//GetWorld()->LineTraceSingleByChannel(_HitOut, _Start, _End, ECC_GameTraceChannel1, _TraceParams);

	//bool isHit = GetWorld()->LineTraceSingleByChannel(_HitOut, _Start, _End, ECC_GameTraceChannel1, _TraceParams);
	bool isHit = GetWorld()->SweepSingleByChannel(_HitOut, _Start, _End, FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(250.f), _TraceParams);
	//DrawDebugLine(GetWorld(), _Start, _End, FColor::Red, false, 10.0f);
	DrawDebugSphere(GetWorld(), _HitOut.ImpactPoint, 10.0f, 12, FColor::Yellow, false, 2.f);

	auto* PlayerActor = Cast<APuckSlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (PlayerActor && isHit)
	{

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("HitActor: %s"), *_HitOut.GetActor()->GetName()));
		PlayerActor->PlayerStatusComponent->TakeDamage(EliteDamageAmount);
		UE_LOG(LogTemp, Warning, TEXT("TakeDamage : %f"), PlayerActor->PlayerStatusComponent->CurrentHealth);
	}
}

float AEliteEnemy::TakeDamage(float takenDamage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{

	EliteEnemyHealth -= takenDamage;
	UE_LOG(LogTemp, Warning, TEXT("%s Remain Health : %f"), *this->GetName(), this->EliteEnemyHealth);

	if (EliteEnemyHealth <= 0)
	{
		FTimerHandle deathTimer;
		GetWorld()->GetTimerManager().SetTimer(deathTimer, this, &AEliteEnemy::Die, 2.0f, false);
	}
	return Super::TakeDamage(takenDamage, DamageEvent, EventInstigator, DamageCauser);

}

void AEliteEnemy::Die()
{
	Destroy();
}

