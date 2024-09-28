// Fill out your copyright notice in the Description page of Project Settings.


#include "PuckSlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "PLauncher.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Blueprint/UserWidget.h"
#include "PBullet.h"
#include "PuckWeaponComponent.h"

// Sets default values
APuckSlayer::APuckSlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	ConstructorHelpers::FObjectFinder<USkeletalMesh> InitMesh(TEXT(""));

	if (InitMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(InitMesh.Object);

		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));

	}

	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocationAndRotation(FVector(0, 0, 50), FRotator(-20, 0, 0));
	springArmComp->TargetArmLength = 250;
	springArmComp->bUsePawnControlRotation = true;


	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	cameraComp->SetupAttachment(springArmComp);
	cameraComp->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = true;

	// Weapon Components
	Rifle = CreateDefaultSubobject<UPuckWeaponComponent>(TEXT("Rifle"));
	Shotgun = CreateDefaultSubobject<UPuckWeaponComponent>(TEXT("Shotgun"));
}

// Called when the game starts or when spawned
void APuckSlayer::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerMappingContext, 0);
		}
	}

	if(IsValid(rifleAimUIFactory))
	{
		if(_rifleAimUI == nullptr)
		{
			_rifleAimUI = CreateWidget(GetWorld(), rifleAimUIFactory);
			_rifleAimUI->AddToViewport();
			_rifleAimUI->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if(IsValid(shotgunAimUIFactory))
	{
		if(_shotgunAimUI == nullptr)
		{
			_shotgunAimUI = CreateWidget(GetWorld(), shotgunAimUIFactory);
			_shotgunAimUI->AddToViewport();
			_shotgunAimUI->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	// Rifle, Shotgun Equipment
	if (bRifle)
		Rifle->AttachWeapon(this);
	if (bShotgun)
		Shotgun->AttachWeapon(this);

	// 줌 관련 변수들 초기화
	DefaultSpringArmLength = springArmComp->TargetArmLength;
	ZoomedSpringArmLength = 100.f;

	DefaultCameraRelativeLocation = cameraComp->GetRelativeLocation();
	ZoomedCameraRelativeLocation = DefaultCameraRelativeLocation + FVector(0.f, -20.f, 0.f);

	// bIsAiming 옵션의 기본 값이 false -> target = default
	TargetSpringArmLength = DefaultSpringArmLength;
	TargetCameraRelativeLocation = DefaultCameraRelativeLocation;
}

// Called every frame
void APuckSlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APuckSlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveIA, ETriggerEvent::Triggered, this, &APuckSlayer::Move);
		EnhancedInputComponent->BindAction(LookUpIA, ETriggerEvent::Triggered, this, &APuckSlayer::LookUp);
		EnhancedInputComponent->BindAction(TurnIA, ETriggerEvent::Triggered, this, &APuckSlayer::Turn);
		EnhancedInputComponent->BindAction(JumpIA, ETriggerEvent::Triggered, this, &APuckSlayer::InputJump);

		EnhancedInputComponent->BindAction(DashIA, ETriggerEvent::Started, this, &APuckSlayer::DashFunc);
		EnhancedInputComponent->BindAction(ZoomIA, ETriggerEvent::Started, this, &APuckSlayer::ZoomFunc);
		EnhancedInputComponent->BindAction(ZoomIA, ETriggerEvent::Completed, this, &APuckSlayer::ZoomOutFunc);
		EnhancedInputComponent->BindAction(ShotgunIA, ETriggerEvent::Started, this, &APuckSlayer::ChangeToShotgun);
		EnhancedInputComponent->BindAction(RifleIA, ETriggerEvent::Started, this, &APuckSlayer::ChangeToRifle);
	}
}

void APuckSlayer::Move(const FInputActionValue& Value)
{
	const FVector MovementVector = Value.Get<FVector>();

	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation = FRotator(0, Rotation.Yaw, 0);
	// make rotation matrix by using Controller's Rotation
	const FRotationMatrix RotationMatrix(YawRotation);

	// get move dircetion
	const FVector ForwardVector = RotationMatrix.GetUnitAxis(EAxis::X);
	const FVector RightVector = RotationMatrix.GetUnitAxis(EAxis::Y);

	const FVector MoveDirection = ForwardVector * MovementVector.Y + RightVector * MovementVector.X;
	AddMovementInput(MoveDirection);
}

void APuckSlayer::LookUp(const FInputActionValue& Value)
{
	float v;
	if (isInvertLookUp)
	{
		v = Value.Get<float>();
	}
	else
	{
		v = Value.Get<float>() * -1;
	}

	
	AddControllerPitchInput(v);
}

void APuckSlayer::Turn(const FInputActionValue& Value)
{
	const float v = Value.Get<float>();
	AddControllerYawInput(v);
}

void APuckSlayer::InputJump(const FInputActionValue& Value)
{
	Jump();
}

void APuckSlayer::DashFunc(const FInputActionValue& value)
{
	FVector LaunchVelocity = GetActorForwardVector() * 1500;
	if (!GetCharacterMovement()->IsFalling())
	{
		GetCharacterMovement()->BrakingFrictionFactor = 0.0f; // 지면 마찰 감소
	}
	LaunchCharacter(LaunchVelocity, true, true);
	
	GetWorldTimerManager().SetTimer(dashTimer, FTimerDelegate::CreateLambda([this]()->void
	{
		GetCharacterMovement()->BrakingFrictionFactor = 2.0f;
	}), 0.5f, false);
}

void APuckSlayer::ZoomFunc(const FInputActionValue& value)
{
	this->bIsAiming = true;
	this->SetWidgetVisible(true, currentEWType);
}

void APuckSlayer::ZoomOutFunc(const FInputActionValue& value)
{
	this->bIsAiming = false;
	this->SetWidgetVisible(false, currentEWType);
}

void APuckSlayer::ChangeToShotgun(const FInputActionValue& value)
{
	currentEWType = EWType::Shotgun;
	this->SetWidgetVisible(false, currentEWType);
	if (SwapMontage) PlayAnimMontage(SwapMontage);
}

void APuckSlayer::ChangeToRifle(const FInputActionValue& value)
{
	currentEWType = EWType::Rifle;
	this->SetWidgetVisible(false, currentEWType);
	if (SwapMontage) PlayAnimMontage(SwapMontage);
}

void APuckSlayer::SetWidgetVisible(bool bVisible,  EWType weaponType)
{
	switch (weaponType)
	{
	case EWType::Shotgun :
		if(bVisible)
		{
			_rifleAimUI->SetVisibility(ESlateVisibility::Hidden);
			_shotgunAimUI->SetVisibility(ESlateVisibility::Visible);
			cameraComp->SetFieldOfView(zoomInFloat);
		}
		else
		{
			_rifleAimUI->SetVisibility(ESlateVisibility::Hidden);
			_shotgunAimUI->SetVisibility(ESlateVisibility::Hidden);
			cameraComp->SetFieldOfView(90.0f);
		}
		break;
	case EWType::Rifle :
		if(bVisible)
		{
			_rifleAimUI->SetVisibility(ESlateVisibility::Visible);
			_shotgunAimUI->SetVisibility(ESlateVisibility::Hidden);
			cameraComp->SetFieldOfView(zoomInFloat);
		}
		else
		{
			_rifleAimUI->SetVisibility(ESlateVisibility::Hidden);
			_shotgunAimUI->SetVisibility(ESlateVisibility::Hidden);
			cameraComp->SetFieldOfView(90.0f);
		}
		break;
	}
}