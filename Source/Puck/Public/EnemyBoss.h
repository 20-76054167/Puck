// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyBoss.generated.h"

UCLASS()
class PUCK_API AEnemyBoss : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBoss();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UPROPERTY(EditAnywhere, Category = "Boss Status")
	float BossHealth;

	void ScaleUp();
	void EnableTick();
	void FollowCharacter();

	float DeltaSum;
	float ScaleUpLimit = 2.0f;

	FVector CurrentScale;
	FVector NewScale;

};
