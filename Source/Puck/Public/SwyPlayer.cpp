// Fill out your copyright notice in the Description page of Project Settings.


#include "SwyPlayer.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ASwyPlayer::ASwyPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ConstructorHelpers::FObjectFinder<USkeletalMesh> initSkel(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonTwinblast/Characters/Heroes/TwinBlast/Meshes/TwinBlast.TwinBlast'"));

	if(initSkel.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(initSkel.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0,0,-86), FRotator(0,-90,0));
	}
	
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->SetRelativeLocationAndRotation(FVector(0,50,65), FRotator(-10,0,0));
	SpringArmComp->TargetArmLength = 250;
	SpringArmComp->bUsePawnControlRotation = true;

	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	cameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	cameraComp->SetRelativeLocationAndRotation(FVector(45,0,22), FRotator(-10,0,0));
	cameraComp->bUsePawnControlRotation = false;

	//bUseControllerRotationYaw = true;
}

// Called when the game starts or when spawned
void ASwyPlayer::BeginPlay()
{
	Super::BeginPlay();

	initChrterWalkSpd = GetCharacterMovement()->MaxWalkSpeed;
	initChrterAcc = GetCharacterMovement()->MaxAcceleration;
	
	if(APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if(UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			SubSystem->AddMappingContext(PlayerMappingContext, 0);
		}
	}
	SetupTimeline();
}

void ASwyPlayer::StartDashFunc(FVector value)
{
	GetCharacterMovement()->MaxWalkSpeed = 2000.0f;
	GetCharacterMovement()->MaxAcceleration = 20000.0f;

	FVector newPos = GetVelocity();
	newPos.Normalize();
	
	AddMovementInput(newPos, 1.0f, false);
}

void ASwyPlayer::EndDashFunc()
{
	GetCharacterMovement()->MaxWalkSpeed = initChrterWalkSpd;
	GetCharacterMovement()->MaxAcceleration = initChrterAcc;
}

void ASwyPlayer::SetupTimeline()
{
	if (Curve)
	{
		DashStartCallback.BindUFunction(this, FName("StartDashFunc"));
		DashEndCallback.BindUFunction(this, FName("EndDashFunc"));
		
		MovementTimeline.AddInterpFloat(Curve, DashStartCallback);
		MovementTimeline.SetTimelineFinishedFunc(DashEndCallback);

		float min = 0;
		float max = 0.5;
		Curve->GetTimeRange(min, max);
		MovementTimeline.SetTimelineLength(max);
	}
}

// Called every frame
void ASwyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MovementTimeline.TickTimeline(DeltaTime);
}

// Called to bind functionality to input
void ASwyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if(UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(moveIA, ETriggerEvent::Triggered, this, &ASwyPlayer::MoveFunc);
		EnhancedInputComponent->BindAction(lookupIA, ETriggerEvent::Triggered, this, &ASwyPlayer::LookupFunc);
		EnhancedInputComponent->BindAction(turnIA, ETriggerEvent::Triggered, this, &ASwyPlayer::TurnFunc);
		EnhancedInputComponent->BindAction(fireIA, ETriggerEvent::Triggered, this, &ASwyPlayer::FireFunc);
		EnhancedInputComponent->BindAction(jumpIA, ETriggerEvent::Triggered, this, &ASwyPlayer::JumpFunc);
		EnhancedInputComponent->BindAction(dashIA, ETriggerEvent::Started, this, &ASwyPlayer::DashFunc);
		EnhancedInputComponent->BindAction(zoomIA, ETriggerEvent::Started, this, &ASwyPlayer::ZoomFunc);
		EnhancedInputComponent->BindAction(zoomIA, ETriggerEvent::Completed, this, &ASwyPlayer::ZoomOutFunc);
	}
}

void ASwyPlayer::MoveFunc(const FInputActionValue& value)
{
	//입력 값을 FVector 형태로 가져옴
	const FVector _CurrentValue = value.Get<FVector>();

	//컨트롤러 Valid Check
	if(Controller)
	{
		//MoveDirection 벡터에 입력된 X, Y 값을 설정
		//언리얼 공식 Doc에서 키배정을 거꾸로 해서 여기서도 반대로?
		moveDirection.X = _CurrentValue.Y;	//좌우 방향 값
		moveDirection.Y = _CurrentValue.X;	//전후 방향 값
	}

	//카메라의 현재 회전에 맞춰서 MoveDirection을 변환
	//[위치 - 스케일 - 회전]
	moveDirection = FTransform(GetControlRotation()).TransformVector(moveDirection);
	AddMovementInput(moveDirection);
	moveDirection = FVector::ZeroVector;
}

void ASwyPlayer::LookupFunc(const FInputActionValue& value)
{
	float _CurrentValue = value.Get<float>() * -1;
	AddControllerPitchInput(_CurrentValue);
}

void ASwyPlayer::TurnFunc(const FInputActionValue& value)
{
	const float _CurrentValue = value.Get<float>();
	AddControllerYawInput(_CurrentValue);
}

void ASwyPlayer::FireFunc(const FInputActionValue& value)
{
}


void ASwyPlayer::JumpFunc(const FInputActionValue& value)
{
	Jump();
}

void ASwyPlayer::DashFunc(const FInputActionValue& value)
{
	//MovementTimeline.PlayFromStart();
	FVector LaunchVelocity = GetActorForwardVector() * 2000;
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

void ASwyPlayer::ZoomFunc(const FInputActionValue& value)
{
	SpringArmComp->TargetArmLength = 150;
}

void ASwyPlayer::ZoomOutFunc(const FInputActionValue& value)
{
	SpringArmComp->TargetArmLength = 250;
}

