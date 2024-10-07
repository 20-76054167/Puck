// Fill out your copyright notice in the Description page of Project Settings.


#include "PuckSlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Blueprint/UserWidget.h"
#include "PuckWeaponComponent.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimSequence.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "FireActorComponent.h"
#include "Puck/ActorComponents/PlayerStatusComponent.h"
#include "Puck/Widgets/HUDUserWidget.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APuckSlayer::APuckSlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->SetRelativeLocationAndRotation(FVector(0, 0, 50), FRotator(-20, 0, 0));
	SpringArmComp->TargetArmLength = 250;
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->SetRelativeLocation(FVector(0.f, 0.f, 0.0f));
	CameraComp->bUsePawnControlRotation = false;

	GetCharacterMovement()->MaxWalkSpeed = 350.0f;

	bUseControllerRotationYaw = true;

	// Weapon Components
	Rifle = CreateDefaultSubobject<UPuckWeaponComponent>(TEXT("Rifle"));
	Shotgun = CreateDefaultSubobject<UPuckWeaponComponent>(TEXT("Shotgun"));

	fireActorComp = CreateDefaultSubobject<UFireActorComponent>(TEXT("FireActorComp"));

	// Player Status Component
	PlayerStatusComponent = CreateDefaultSubobject<UPlayerStatusComponent>(TEXT("PlayerStatusComponent"));
	
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

	if(IsValid(RifleAimUIFactory))
	{
		if(RifleAimUI == nullptr)
		{
			RifleAimUI = CreateWidget(GetWorld(), RifleAimUIFactory);
			RifleAimUI->AddToViewport();
			RifleAimUI->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if(IsValid(ShotgunAimUIFactory))
	{
		if(ShotgunAimUI == nullptr)
		{
			ShotgunAimUI = CreateWidget(GetWorld(), ShotgunAimUIFactory);
			ShotgunAimUI->AddToViewport();
			ShotgunAimUI->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if(IsValid(NormalAimUIFactory))
	{
		if(NormalAimUI == nullptr)
		{
			NormalAimUI = CreateWidget(GetWorld(), NormalAimUIFactory);
			NormalAimUI->AddToViewport();
		}
	}

	// HUD
	if (IsValid(HUDFactory))
	{
		if (HUD == nullptr)
		{
			HUD = Cast<UHUDUserWidget>(CreateWidget(GetWorld(), HUDFactory));
			HUD->AddToViewport();
		}
	}
	
	// Rifle, Shotgun Equipment
	if (bRifle)
	{
		Rifle->AttachWeapon(this);
		// 라이플을 먼저 장착했으니 라이플 상태로 설정
		CurrentEwType = EWType::Rifle;
		HUD->SetWeaponIcon(Rifle->WeaponIcon);
		fireActorComp->ChangeActorMode(CurrentEwType);
	}
	if (bShotgun)
		Shotgun->AttachWeapon(this);

	// Zoom 변수 초기화
	// Default
	DefaultSpringArmLength = SpringArmComp->TargetArmLength;
	DefaultCameraRelativeLocation = CameraComp->GetRelativeLocation();

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

	// bIsAiming 에 따라 보간 목표를 진행한다
	float NewArmLength = FMath::FInterpTo(SpringArmComp->TargetArmLength, TargetSpringArmLength, DeltaTime, ZoomInterpSpeed);
	SpringArmComp->TargetArmLength = NewArmLength;

	FVector NewCameraRelativeLocation = FMath::VInterpTo(CameraComp->GetRelativeLocation(), TargetCameraRelativeLocation, DeltaTime, ZoomInterpSpeed);
	CameraComp->SetRelativeLocation(NewCameraRelativeLocation);
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
		EnhancedInputComponent->BindAction(ExecutionIA, ETriggerEvent::Started, this, &APuckSlayer::Execution);
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
	if (bIsInvertLookUp)
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

void APuckSlayer::DashFunc(const FInputActionValue& Value)
{
	if (DashSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(),DashSound, GetActorLocation());
	}
	FVector Velocity = GetCharacterMovement()->Velocity;
	Velocity.Z = 0.0;
	Velocity.Normalize();
	
	FVector LaunchVelocity = Velocity * 1500;
	if (!GetCharacterMovement()->IsFalling())
	{
		GetCharacterMovement()->BrakingFrictionFactor = 0.0f; // 지면 마찰 감소
	}
	LaunchCharacter(LaunchVelocity, true, false);
	
	GetWorldTimerManager().SetTimer(DashTimer, FTimerDelegate::CreateLambda([this]()->void
	{
		GetCharacterMovement()->BrakingFrictionFactor = 2.0f;
	}), 0.5f, false);

}

void APuckSlayer::ZoomFunc(const FInputActionValue& Value)
{
	if (bIsRunning) return;
	
	this->bIsAiming = true;
	this->SetWidgetVisible(true, CurrentEwType);

	switch (CurrentEwType)
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

void APuckSlayer::ZoomOutFunc(const FInputActionValue& Value)
{
	// if (bIsRunning) return;
	
	this->bIsAiming = false;
	this->SetWidgetVisible(false, CurrentEwType);

	TargetSpringArmLength = DefaultSpringArmLength;
	TargetCameraRelativeLocation = DefaultCameraRelativeLocation;
}

void APuckSlayer::ChangeToShotgun(const FInputActionValue& Value)
{
	// 이미 shotgun 인 경우 무시
	if (CurrentEwType == EWType::Shotgun) return;

	// SwapAnimNotifyState 클래스에서 타입 변경
	// 이렇게 해야 스왑 중 다른 무기로 스왑하는 것을 막을 수 있음
	// currentEWType = EWType::Shotgun;
	
	this->SetWidgetVisible(false, CurrentEwType);
	if (SwapMontage) PlayAnimMontage(SwapMontage);
	
	fireActorComp->ChangeActorMode(EWType::Shotgun);
}

void APuckSlayer::ChangeToRifle(const FInputActionValue& Value)
{
	// 이미 rifle 인 경우 무시
	if (CurrentEwType == EWType::Rifle) return;

	// SwapAnimNotifyState 클래스에서 타입 변경
	// 이렇게 해야 스왑 중 다른 무기로 스왑하는 것을 막을 수 있음
	// currentEWType = EWType::Rifle;
	
	this->SetWidgetVisible(false, CurrentEwType);
	if (SwapMontage) PlayAnimMontage(SwapMontage);

	fireActorComp->ChangeActorMode(EWType::Rifle);
}

void APuckSlayer::RunStart(const FInputActionValue& Value)
{
	// ZoomOutFunc 내용인데 어캐 넣지 고민하다가 일단 그냥 넣음, 수정해야함
	if (bIsAiming)
	{
		this->bIsAiming = false;
		this->SetWidgetVisible(false, CurrentEwType);

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

void APuckSlayer::RunEnd(const FInputActionValue& Value)
{
	bIsRunning = false;
	GetCharacterMovement()->MaxWalkSpeed = 350.0f;
}

void APuckSlayer::Execution(const FInputActionValue& Value)
{
	if (ExecutionStab && GetMesh())
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			float MontageResult = AnimInstance->Montage_Play(ExecutionStab);
		}
	}
}

void APuckSlayer::SetWidgetVisible(bool bVisible,  EWType WeaponType)
{
	switch (WeaponType)
	{
	case EWType::Shotgun :
		if(bVisible)
		{
			RifleAimUI->SetVisibility(ESlateVisibility::Hidden);
			ShotgunAimUI->SetVisibility(ESlateVisibility::Visible);
			NormalAimUI->SetVisibility(ESlateVisibility::Hidden);
			// cameraComp->SetFieldOfView(zoomInFloat);
		}
		else
		{
			RifleAimUI->SetVisibility(ESlateVisibility::Hidden);
			ShotgunAimUI->SetVisibility(ESlateVisibility::Hidden);
			NormalAimUI->SetVisibility(ESlateVisibility::Visible);
			// cameraComp->SetFieldOfView(90.0f);
		}
		break;
	case EWType::Rifle :
		if(bVisible)
		{
			RifleAimUI->SetVisibility(ESlateVisibility::Visible);
			ShotgunAimUI->SetVisibility(ESlateVisibility::Hidden);
			NormalAimUI->SetVisibility(ESlateVisibility::Hidden);
			// cameraComp->SetFieldOfView(zoomInFloat);
		}
		else
		{
			RifleAimUI->SetVisibility(ESlateVisibility::Hidden);
			ShotgunAimUI->SetVisibility(ESlateVisibility::Hidden);
			NormalAimUI->SetVisibility(ESlateVisibility::Visible);
			// cameraComp->SetFieldOfView(90.0f);
		}
		break;
	}
}

void APuckSlayer::PlayFireAnim()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance)
	{
		int32 playerMagazine = fireActorComp->GetCurrentMagazine();
		if(playerMagazine > 0)
		{
			if(fireActorComp->bCanAttack)
			{
				if(CurrentEwType == EWType::Shotgun)
				{
					if(bIsAiming)
					{
						AnimInstance->Montage_Play(ZoomFireShotgunAnim);
					}
					else
					{
						AnimInstance->Montage_Play(FireShotgunAnim);
					}
				}
				else if(CurrentEwType == EWType::Rifle)
				{
					if(bIsAiming)
					{
						AnimInstance->Montage_Play(ZoomFireRifleAnim);
					}
					else
					{
						AnimInstance->Montage_Play(FireRifleAnim);
					}
				}
			}
		}
		else
		{
			this->PlayReloadAnim();
		}  
	}
}

void APuckSlayer::PlayReloadAnim()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance)
	{
		if(CurrentEwType == EWType::Shotgun)
		{
			AnimInstance->Montage_Play(ReloadShotgunAnim);	
		}
		else if(CurrentEwType == EWType::Rifle)
		{
			AnimInstance->Montage_Play(ReloadRifleAnim);
		}
	}
}



// player status component 에서 체력을 처리하기 때문에 주석 처리
// float APuckSlayer::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
// {
// 	SlayerHealth -= DamageAmount;
//
// 	UE_LOG(LogTemp, Warning, TEXT("Current_SlayerHealth : %d"), SlayerHealth);
// 	if (SlayerHealth <= 0.0f)
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("Slayer Die!!!!!"));
// 	}
// 	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
// }
