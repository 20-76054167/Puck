// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PBullet.generated.h"

UCLASS()
class PUCK_API APBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
	class UProjectileMovementComponent* PMovementComp;

	UPROPERTY(VisibleAnywhere, Category = "Collider")
	class USphereComponent* SphereCollComp;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	class UStaticMeshComponent* SMeshComp;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
