// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Components/TimelineComponent.h"
#include "Puck/PuckCharacter.h"
#include "SwyPlayer.generated.h"

UCLASS()
class PUCK_API ASwyPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASwyPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Camera")
	class USpringArmComponent* SpringArmComp;
	
	UPROPERTY(EditAnywhere, Category = "Camera")
	class UCameraComponent* cameraComp;

	UPROPERTY(VisibleAnywhere, Category = "MovementOption")
	float initChrterWalkSpd = 0.0f;

	UPROPERTY(VisibleAnywhere, Category = "MovementOption")
	float initChrterAcc = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* PlayerMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* moveIA;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* lookupIA;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* turnIA;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* zoomIA;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* fireIA;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* jumpIA;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* dashIA;

	UPROPERTY(EditAnywhere, Category="Fire")
	class UAnimMontage* FireMontage; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Curve")
	class UCurveFloat* Curve;

	FTimeline MovementTimeline;
	FOnTimelineFloat DashStartCallback;
	FOnTimelineEvent DashEndCallback;

	void MoveFunc(const FInputActionValue& value);
	void LookupFunc(const FInputActionValue& value);
	void TurnFunc(const FInputActionValue& value);
	void FireFunc(const FInputActionValue& value);
	void JumpFunc(const FInputActionValue& value);
	void DashFunc(const FInputActionValue& value);
	void ZoomFunc(const FInputActionValue& value);
	void ZoomOutFunc(const FInputActionValue& value);

	void Fire();
	
	UFUNCTION()
	void StartDashFunc(FVector value);
	UFUNCTION()
	void EndDashFunc();
	void SetupTimeline();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	FVector moveDirection;
	FTimerHandle dashTimer;
	UCharacterMovementComponent* moveComp;

};
