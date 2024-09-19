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

	WeaponMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMeshComp->SetupAttachment(GetMesh());
	
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
		EnhancedInputComponent->BindAction(FireIA, ETriggerEvent::Started, this, &APuckSlayer::InputFire);

		EnhancedInputComponent->BindAction(DashIA, ETriggerEvent::Started, this, &APuckSlayer::DashFunc);
		EnhancedInputComponent->BindAction(ZoomIA, ETriggerEvent::Started, this, &APuckSlayer::ZoomFunc);
		EnhancedInputComponent->BindAction(ZoomIA, ETriggerEvent::Completed, this, &APuckSlayer::ZoomOutFunc);
		EnhancedInputComponent->BindAction(ShotgunIA, ETriggerEvent::Started, this, &APuckSlayer::ChangeToShotgun);
		EnhancedInputComponent->BindAction(RifleIA, ETriggerEvent::Started, this, &APuckSlayer::ChangeToRifle);
	}

}

void APuckSlayer::Move(const FInputActionValue& Value)
{
	const FVector v = Value.Get<FVector>();
	if (Controller)
	{
		MoveDirection.Y = v.X;
		MoveDirection.X = v.Y;
	}

	MoveDirection = FTransform(GetControlRotation()).TransformVector(MoveDirection);
	AddMovementInput(MoveDirection);
	MoveDirection = FVector::ZeroVector;
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

void APuckSlayer::InputFire(const FInputActionValue& Value)
{
	FTransform firePosition = WeaponMeshComp->GetSocketTransform(TEXT("FirePosition"));
	GetWorld()->SpawnActor<APBullet>(magazine, firePosition);
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
	//SpringArmComp->TargetArmLength = 150;
	if(IsValid(_rifleAimUI))
	{
		////_rifleAimUI->AddToViewport();
		//_rifleAimUI->SetVisibility(ESlateVisibility::Visible);
		//cameraComp->SetFieldOfView(zoomInFloat);
	}

	if(IsValid(_shotgunAimUI))
	{
		//_shotgunAimUI->AddToViewport();
		_shotgunAimUI->SetVisibility(ESlateVisibility::Visible);
		cameraComp->SetFieldOfView(zoomInFloat);
	}
}

void APuckSlayer::ZoomOutFunc(const FInputActionValue& value)
{
	//SpringArmComp->TargetArmLength = 250;
	if(IsValid(_rifleAimUI))
	{
		////_rifleAimUI->RemoveFromParent();
		//_rifleAimUI->SetVisibility(ESlateVisibility::Hidden);
		//cameraComp->SetFieldOfView(90.0f);
	}

	if(IsValid(_shotgunAimUI))
	{
		_shotgunAimUI->SetVisibility(ESlateVisibility::Hidden);
		cameraComp->SetFieldOfView(90.0f);
	}
}

void APuckSlayer::ChangeToShotgun(const FInputActionValue& value)
{
}

void APuckSlayer::ChangeToRifle(const FInputActionValue& value)
{
}
