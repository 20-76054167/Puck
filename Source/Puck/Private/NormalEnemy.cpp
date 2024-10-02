// Fill out your copyright notice in the Description page of Project Settings.


#include "NormalEnemy.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ANormalEnemy::ANormalEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	EnemyHealth = 100.0f;


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
			
			//적이 플레이어 바라보기
			FVector RotateDirection = PlayerLocation - ThisEnemyLoaction;
			FRotator NewRotation = RotateDirection.Rotation();
			SetActorRotation(NewRotation);

			if (DistanceToPlayer <= 200.0f)
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
	
	bool isHit = GetWorld()->LineTraceSingleByChannel(_HitOut, _Start, _End, ECC_GameTraceChannel1, _TraceParams);
	DrawDebugLine(GetWorld(), _Start, _End, FColor::Red, false, 10.0f);
	DrawDebugSphere(GetWorld(), _HitOut.ImpactPoint, 10.0f, 12, FColor::Yellow, false, 2.f);
	
	AActor* PlayerActor = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (PlayerActor && isHit)
	{
		
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("HitActor: %s"), *_HitOut.GetActor()->GetName()));
		UGameplayStatics::ApplyDamage(PlayerActor, DamageAmount, GetController(), this, UDamageType::StaticClass());
		UE_LOG(LogTemp, Warning, TEXT("TakeDamage : %f"), DamageAmount);
	}


}

