// Fill out your copyright notice in the Description page of Project Settings.


#include "NormalEnemy.h"
#include "PuckSlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Puck/ActorComponents/PlayerStatusComponent.h"

// Sets default values
ANormalEnemy::ANormalEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	EnemyHealth = 30.f;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonGrux/FX/Particles/Skins/Grux_Beetle_Magma/P_Grux_Magma_Ultimate_Cast.P_Grux_Magma_Ultimate_Cast'")); // 실제 파티클 경로로 수정

	if (ParticleAsset.Succeeded())
	{
		EparticleEffect = ParticleAsset.Object; // 파티클 이펙트를 변수에 할당
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleDeathAsset(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonGrux/FX/Particles/Skins/Grux_Beetle_Magma/P_Grux_Magma_Spawn.P_Grux_Magma_Spawn'")); // 실제 파티클 경로로 수정

	if (ParticleDeathAsset.Succeeded())
	{
		EDeathEffect = ParticleDeathAsset.Object; // 파티클 이펙트를 변수에 할당
	}

}

// Called when the game starts or when spawned
void ANormalEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANormalEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//EnemyFollowCharacter();

}

// Called to bind functionality to input
void ANormalEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ANormalEnemy::EnemyFollowCharacter()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		APawn* PlayerPawn = PlayerController->GetPawn();
		if (PlayerPawn)
		{
			FVector PlayerLocation = PlayerPawn->GetActorLocation();
			FVector ThisEnemyLoaction = GetActorLocation();

			float DistanceToPlayer = FVector::Dist(PlayerLocation, ThisEnemyLoaction);
			
			//���� �÷��̾� �ٶ󺸱�
			FVector RotateDirection = PlayerLocation - ThisEnemyLoaction;
			FRotator NewRotation = RotateDirection.Rotation();
			SetActorRotation(NewRotation);

			if (DistanceToPlayer <= 200.0f )
			{
				AttackPlayer();
			}
			
				
			FVector FollowDirection = PlayerLocation - ThisEnemyLoaction;
			FollowDirection.Normalize();
			FVector FollowLocation = GetActorLocation() - FollowDirection * 5;
			FollowDirection = FTransform(GetControlRotation()).TransformVector(FollowDirection);
			AddMovementInput(FollowDirection);
			//FollowDirection = FVector::ZeroVector;
			//SetActorLocation(FollowLocation);
				

			
		}
		
	}
}

void ANormalEnemy::AttackPlayer()
{
	FVector _Start = GetActorLocation();
	FVector _End = GetActorLocation() + GetActorForwardVector() * 300.0f;
	FHitResult _HitOut;

	FCollisionQueryParams _TraceParams;
	_TraceParams.AddIgnoredActor(this);
	//GetWorld()->LineTraceSingleByChannel(_HitOut, _Start, _End, ECC_GameTraceChannel1, _TraceParams);
	
	bool isHit = GetWorld()->SweepSingleByChannel(_HitOut, _Start, _End, FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(200.f), _TraceParams);
	//DrawDebugLine(GetWorld(), _Start, _End, FColor::Red, false, 10.0f);
	DrawDebugSphere(GetWorld(), _HitOut.ImpactPoint, 10.0f, 12, FColor::Yellow, false, 2.f);
	FVector hitLocation = _HitOut.ImpactPoint;
	
	auto* PlayerActor = Cast<APuckSlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (PlayerActor && isHit)
	{
		if (EparticleEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EparticleEffect, hitLocation, FRotator::ZeroRotator, true);
		}
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("HitActor: %s"), *_HitOut.GetActor()->GetName()));
		// UGameplayStatics::ApplyDamage(PlayerActor, DamageAmount, GetController(), this, UDamageType::StaticClass());
		PlayerActor->PlayerStatusComponent->TakeDamage(DamageAmount);
		UE_LOG(LogTemp, Warning, TEXT("TakeDamage : %f"), PlayerActor->PlayerStatusComponent->CurrentHealth);
	}


}

float ANormalEnemy::TakeDamage(float takenDamage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	EnemyHealth -= takenDamage;

	UE_LOG(LogTemp, Warning, TEXT("%s Remain Health : %f"), *this->GetName(), this->EnemyHealth);
	if (EnemyHealth <= 0)
	{
		FTimerHandle deathTimer;
		GetWorld()->GetTimerManager().SetTimer(deathTimer, this, &ANormalEnemy::Die, 2.0f, false);
	}
	return Super::TakeDamage(takenDamage, DamageEvent, EventInstigator, DamageCauser);
}

void ANormalEnemy::Die()
{
	FVector newDieScale(6.0f, 6.0f, 6.0f);
	if (EDeathEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EDeathEffect, GetActorLocation(), FRotator::ZeroRotator, newDieScale, true);
	}
	Destroy();
}




