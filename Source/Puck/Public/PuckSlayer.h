// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "EWType.h"
#include "PuckSlayer.generated.h"

class UPuckWeaponComponent;
class UInputMappingContext;
class UInputAction;
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class USpringArmComponent* springArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* cameraComp;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* PlayerMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveIA;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookUpIA;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* TurnIA;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* JumpIA;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* ZoomIA;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* DashIA;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* ShotgunIA;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* RifleIA;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* RunIA;


	UPROPERTY(EditAnywhere, Category = "Fire")
	TSubclassOf<class APBullet> magazine;

	void Move(const FInputActionValue& Value);
	void LookUp(const FInputActionValue& Value);
	void Turn(const FInputActionValue& Value);
	void InputJump(const FInputActionValue& Value);
	void DashFunc(const FInputActionValue& value);
	void ZoomFunc(const FInputActionValue& value);
	void ZoomOutFunc(const FInputActionValue& value);
	void ChangeToShotgun(const FInputActionValue& value);
	void ChangeToRifle(const FInputActionValue& value);
	void RunStart(const FInputActionValue& value);
	void RunEnd(const FInputActionValue& value);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control option")
	bool isInvertLookUp = false;

	UPROPERTY(EditAnywhere)
	class APLauncher* PLauncher;

	UPROPERTY(EditAnywhere, Category="Widget")
	TSubclassOf<class UUserWidget> normalAimUIFactory;
	class UUserWidget* _normalAimUI;

	UPROPERTY(EditAnywhere, Category="Widget")
	TSubclassOf<class UUserWidget> rifleAimUIFactory;
	class UUserWidget* _rifleAimUI;

	UPROPERTY(EditAnywhere, Category="Widget")
	TSubclassOf<class UUserWidget> shotgunAimUIFactory;
	class UUserWidget* _shotgunAimUI;
	
	UPROPERTY(EditAnywhere, Category="Widget")
	float zoomInFloat = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="State")
	EWType currentEWType;

	FTimerHandle dashTimer;
	
	void SetWidgetVisible(bool bVisible, EWType weaponType);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	int32 OwingWeaponNum = 0;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* SwapMontage;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	UPuckWeaponComponent* Rifle;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	UPuckWeaponComponent* Shotgun;

private:
	// 줌(우클릭) 하고 있는지를 추적하는 bool 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
	bool bIsAiming = false;
	
	UPROPERTY(EditAnywhere, Category = "Weapon")
	bool bRifle = true;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	bool bShotgun = true;

	// 줌인 - 줌아웃 관련 변수들

	// Default SpringArmLength
	float DefaultSpringArmLength;
	FVector DefaultCameraRelativeLocation;

	// Rifle
	float ZoomedRifleSpringArmLength;
	FVector ZoomedRifleCameraRelativeLocation;

	// Shotgun
	float ZoomedShotgunSpringArmLength;
	FVector ZoomedShotgunCameraRelativeLocation;

	// Target
	float TargetSpringArmLength;
	FVector TargetCameraRelativeLocation;
	
	// Interpolation 속도
	float ZoomInterpSpeed = 10.f;

	// running
	bool bIsRunning = false;
	
};
