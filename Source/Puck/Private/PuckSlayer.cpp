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
	cameraComp->SetRelativeLocation(FVector(0.f, 0.f, 0.0f));
	cameraComp->bUsePawnControlRotation = false;

	GetCharacterMovement()->MaxWalkSpeed = 350.0f;

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

	if(IsValid(normalAimUIFactory))
	{
		if(_normalAimUI == nullptr)
		{
			_normalAimUI = CreateWidget(GetWorld(), normalAimUIFactory);
			_normalAimUI->AddToViewport();
		}
	}
	
	// Rifle, Shotgun Equipment
	if (bRifle)
	{
		Rifle->AttachWeapon(this);
		// 라이플을 먼저 장착했으니 라이플 상태로 설정
		currentEWType = EWType::Rifle;
	}
	if (bShotgun)
		Shotgun->AttachWeapon(this);

	

	// Zoom 변수 초기화
	// Default
	DefaultSpringArmLength = springArmComp->TargetArmLength;
	DefaultCameraRelativeLocation = cameraComp->GetRelativeLocation();

	// Rifle
	ZoomedRifleSpringArmLength = 150.0f;
	ZoomedRifleCameraRelativeLocation = DefaultCameraRelativeLocation + FVector(0.f, 30.f, 0.f);

	// Shotgun
	ZoomedShotgunSpringArmLength = 200.0f;
	ZoomedShotgunCameraRelativeLocation = DefaultCameraRelativeLocation + FVector(0.f, 15.f, 0.f);

	// bIsAiming 옵션의 기본 값이 false -> target = default
	TargetSpringArmLength = DefaultSpringArmLength;
	TargetCameraRelativeLocation = DefaultCameraRelativeLocation;
}

// Called every frame
void APuckSlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float NewArmLength = FMath::FInterpTo(springArmComp->TargetArmLength, TargetSpringArmLength, DeltaTime, ZoomInterpSpeed);
	springArmComp->TargetArmLength = NewArmLength;

	FVector NewCameraRelativeLocation = FMath::VInterpTo(cameraComp->GetRelativeLocation(), TargetCameraRelativeLocation, DeltaTime, ZoomInterpSpeed);
	cameraComp->SetRelativeLocation(NewCameraRelativeLocation);
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

		// Run Start and End
		EnhancedInputComponent->BindAction(RunIA, ETriggerEvent::Started, this, &APuckSlayer::RunStart);
		EnhancedInputComponent->BindAction(RunIA, ETriggerEvent::Completed, this, &APuckSlayer::RunEnd);
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
	FVector Velocity = GetCharacterMovement()->Velocity;
	Velocity.Z = 0.0;
	Velocity.Normalize();
	
	FVector LaunchVelocity = Velocity * 1500;
	if (!GetCharacterMovement()->IsFalling())
	{
		GetCharacterMovement()->BrakingFrictionFactor = 0.0f; // 지면 마찰 감소
	}
	LaunchCharacter(LaunchVelocity, true, false);
	
	GetWorldTimerManager().SetTimer(dashTimer, FTimerDelegate::CreateLambda([this]()->void
	{
		GetCharacterMovement()->BrakingFrictionFactor = 2.0f;
	}), 0.5f, false);
}

void APuckSlayer::ZoomFunc(const FInputActionValue& value)
{
	if (bIsRunning) return;
	
	this->bIsAiming = true;
	this->SetWidgetVisible(true, currentEWType);

	switch (currentEWType)
	{
	case EWType::Shotgun:
		TargetSpringArmLength = ZoomedShotgunSpringArmLength;
		TargetCameraRelativeLocation = ZoomedShotgunCameraRelativeLocation;
		break;
	case EWType::Rifle:
		TargetSpringArmLength = ZoomedRifleSpringArmLength;
		TargetCameraRelativeLocation = ZoomedRifleCameraRelativeLocation;
		break;
	default:
		break;
	}
}

void APuckSlayer::ZoomOutFunc(const FInputActionValue& value)
{
	// if (bIsRunning) return;
	
	this->bIsAiming = false;
	this->SetWidgetVisible(false, currentEWType);

	TargetSpringArmLength = DefaultSpringArmLength;
	TargetCameraRelativeLocation = DefaultCameraRelativeLocation;
}

void APuckSlayer::ChangeToShotgun(const FInputActionValue& value)
{
	// 이미 shotgun 인 경우 무시
	if (currentEWType == EWType::Shotgun) return;

	// SwapAnimNotifyState 클래스에서 타입 변경
	// 이렇게 해야 스왑 중 다른 무기로 스왑하는 것을 막을 수 있음
	// currentEWType = EWType::Shotgun;
	
	this->SetWidgetVisible(false, currentEWType);
	if (SwapMontage) PlayAnimMontage(SwapMontage);
}

void APuckSlayer::ChangeToRifle(const FInputActionValue& value)
{
	// 이미 rifle 인 경우 무시
	if (currentEWType == EWType::Rifle) return;

	// SwapAnimNotifyState 클래스에서 타입 변경
	// 이렇게 해야 스왑 중 다른 무기로 스왑하는 것을 막을 수 있음
	// currentEWType = EWType::Rifle;
	
	this->SetWidgetVisible(false, currentEWType);
	if (SwapMontage) PlayAnimMontage(SwapMontage);
}

void APuckSlayer::RunStart(const FInputActionValue& value)
{
	// ZoomOutFunc 내용인데 어캐 넣지 고민하다가 일단 그냥 넣음, 수정해야함
	if (bIsAiming)
	{
		this->bIsAiming = false;
		this->SetWidgetVisible(false, currentEWType);

		TargetSpringArmLength = DefaultSpringArmLength;
		TargetCameraRelativeLocation = DefaultCameraRelativeLocation;
	}
	//
	
	bIsRunning = true;
	
	if (bIsAiming)
	{
		bIsAiming = false;
	}
	
	GetCharacterMovement()->MaxWalkSpeed = 700.0f;
}

void APuckSlayer::RunEnd(const FInputActionValue& value)
{
	bIsRunning = false;
	GetCharacterMovement()->MaxWalkSpeed = 350.0f;
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
			_normalAimUI->SetVisibility(ESlateVisibility::Hidden);
			// cameraComp->SetFieldOfView(zoomInFloat);
		}
		else
		{
			_rifleAimUI->SetVisibility(ESlateVisibility::Hidden);
			_shotgunAimUI->SetVisibility(ESlateVisibility::Hidden);
			_normalAimUI->SetVisibility(ESlateVisibility::Visible);
			// cameraComp->SetFieldOfView(90.0f);
		}
		break;
	case EWType::Rifle :
		if(bVisible)
		{
			_rifleAimUI->SetVisibility(ESlateVisibility::Visible);
			_shotgunAimUI->SetVisibility(ESlateVisibility::Hidden);
			_normalAimUI->SetVisibility(ESlateVisibility::Hidden);
			// cameraComp->SetFieldOfView(zoomInFloat);
		}
		else
		{
			_rifleAimUI->SetVisibility(ESlateVisibility::Hidden);
			_shotgunAimUI->SetVisibility(ESlateVisibility::Hidden);
			_normalAimUI->SetVisibility(ESlateVisibility::Visible);
			// cameraComp->SetFieldOfView(90.0f);
		}
		break;
	}
}
