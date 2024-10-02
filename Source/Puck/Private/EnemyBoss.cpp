// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBoss.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyBoss::AEnemyBoss()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BossHealth = 100.0f;
	DeltaSum = 0;
	ConstructorHelpers::FObjectFinder<USkeletalMesh> InitMesh(TEXT(""));
	if (InitMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(InitMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}



}

// Called when the game starts or when spawned
void AEnemyBoss::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false);
	FTimerHandle ScaleUpTimerHandle;

	GetWorld()->GetTimerManager().SetTimer(ScaleUpTimerHandle, this, &AEnemyBoss::EnableTick, 4.0f, false);
	
}


// Called every frame
void AEnemyBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	ScaleUp();

	
	//FollowCharacter();



}

// Called to bind functionality to input
void AEnemyBoss::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyBoss::ScaleUp()
{
	if (ScaleUpLimit > DeltaSum) {
		CurrentScale = GetActorScale();

		NewScale = CurrentScale * 1.003f;

		SetActorScale3D(NewScale);

		DeltaSum += GetWorld()->DeltaTimeSeconds;
	}
	else
	{
		return;
	}


}

void AEnemyBoss::EnableTick()
{
	SetActorTickEnabled(true);
}

void AEnemyBoss::FollowCharacter()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		APawn* PlayerPawn = PlayerController->GetPawn();
		if (PlayerPawn)
		{
			FVector PlayerLoaction = PlayerPawn->GetActorLocation();
			FVector BossLocation = GetActorLocation();

			FVector Direction = PlayerLoaction - BossLocation;
			FVector FollowD = BossLocation - PlayerLoaction;
			FollowD.Normalize();
			FVector FollowLocation = GetActorLocation() - FollowD * 3;


			FRotator NewRotation = Direction.Rotation();

			SetActorLocation(FollowLocation);
			SetActorRotation(NewRotation);
		}
	}
}

