// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"

#include "PuckSlayer.generated.h"

UCLASS()
class PUCK_API APuckSlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APuckSlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Mesh")
	class UStaticMeshComponent* WeaponMesh;
	
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class UCameraComponent* CameraComp;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputMappingContext* PlayerMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* IA_Move;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_LookUp;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Turn;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Jump;
	
	void Move(const FInputActionValue& Value);
	void LookUp(const FInputActionValue& Value);
	void Turn(const FInputActionValue& Value);
	void _Jump(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Camera Options")
	bool IsInvertLookUp = false;
private:
	FVector MoveDirection;
};
