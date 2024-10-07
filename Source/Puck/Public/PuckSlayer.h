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
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* CameraComp;

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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* ExecutionIA;

	// Player Status Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ActorComponent")
	class UPlayerStatusComponent* PlayerStatusComponent;


	UPROPERTY(EditAnywhere, Category = "Fire")
	TSubclassOf<class APBullet> Magazine;

	void Move(const FInputActionValue& Value);
	void LookUp(const FInputActionValue& Value);
	void Turn(const FInputActionValue& Value);
	void InputJump(const FInputActionValue& Value);
	void DashFunc(const FInputActionValue& Value);
	void ZoomFunc(const FInputActionValue& Value);
	void ZoomOutFunc(const FInputActionValue& Value);
	void ChangeToShotgun(const FInputActionValue& Value);
	void ChangeToRifle(const FInputActionValue& Value);
	void RunStart(const FInputActionValue& Value);
	void RunEnd(const FInputActionValue& Value);
	void Execution(const FInputActionValue& Value);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control option")
	bool bIsInvertLookUp = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Execution")
	class UAnimMontage* ExecutionStab;
	
	UPROPERTY(EditAnywhere)
	class APLauncher* PLauncher;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widget")
	class UWidgetAnimation* rifleZoomAnim;

	UPROPERTY(EditAnywhere, Category="Widget")
	TSubclassOf<class UUserWidget> NormalAimUIFactory;
	class UUserWidget* NormalAimUI;

	UPROPERTY(EditAnywhere, Category="Widget")
	TSubclassOf<class UUserWidget> RifleAimUIFactory;
	class UUserWidget* RifleAimUI;

	UPROPERTY(EditAnywhere, Category="Widget")
	TSubclassOf<class UUserWidget> ShotgunAimUIFactory;
	class UUserWidget* ShotgunAimUI;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<class UHUDUserWidget> HUDFactory;
	class UHUDUserWidget* HUD;
	
	UPROPERTY(EditAnywhere, Category="Widget")
	float ZoomInFloat = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="State")
	EWType CurrentEwType;

	FTimerHandle DashTimer;
	
	void SetWidgetVisible(bool bVisible, EWType WeaponType);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	int32 OwingWeaponNum = 0;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* SwapMontage;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	UPuckWeaponComponent* Rifle;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	UPuckWeaponComponent* Shotgun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UFireActorComponent* fireActorComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimMontage")
	class UAnimMontage* FireShotgunAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimMontage")
	class UAnimMontage* ZoomFireShotgunAnim;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimMontage")
	class UAnimMontage* ReloadShotgunAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimMontage")
	class UAnimMontage* FireRifleAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimMontage")
	class UAnimMontage* ZoomFireRifleAnim;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimMontage")
	class UAnimMontage* ReloadRifleAnim;

	void PlayFireAnim();
	void PlayReloadAnim();
	
	// player status component 에서 체력을 처리하기 때문에 주석 처리
	// float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:
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

public:
	// Dash Cooldown Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Dash")
	float DashCooldownTime = 2.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dash")
	bool bIsDashOnCooldown = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dash")
	float DashRemainingTime = 0.0f;

	// Function to reset dash cooldown
	void ResetDashCooldown();
};
