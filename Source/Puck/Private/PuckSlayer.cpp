
#include "PuckSlayer.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"

APuckSlayer::APuckSlayer()
{
	PrimaryActorTick.bCanEverTick = true;
	
	ConstructorHelpers::FObjectFinder<USkeletalMesh>InitMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/ParagonMurdock/Characters/Heroes/Murdock/Skins/CS_MatteBlack/Mesh/Murdock_SF.Murdock_SF'"));
	if(InitMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(InitMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0,0,-88), FRotator(0,-90,0));
	}

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->SetRelativeLocationAndRotation(FVector(0,0,50),FRotator(-20, 0 ,0));
	SpringArmComp->TargetArmLength = 530;
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->bUsePawnControlRotation = false;

	// WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Mesh"));
	// WeaponMesh->SetupAttachment(GetMesh(), FName(TEXT("Character1_RightHandSocket")));
	
	bUseControllerRotationYaw = true;
}

void APuckSlayer::BeginPlay()
{
	Super::BeginPlay();

	if(APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext((PlayerMappingContext),0);
		}
	}
}

void APuckSlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APuckSlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &APuckSlayer::Move);
		EnhancedInputComponent->BindAction(IA_LookUp, ETriggerEvent::Triggered, this, &APuckSlayer::LookUp);
		EnhancedInputComponent->BindAction(IA_Turn, ETriggerEvent::Triggered, this, &APuckSlayer::Turn);

		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this, &APuckSlayer::_Jump);
	}
}

void APuckSlayer::Move(const FInputActionValue& Value)
{
	const FVector _CurrentValue = Value.Get<FVector>();
	if(Controller)
	{
		MoveDirection.X = _CurrentValue.Y;
		MoveDirection.Y = _CurrentValue.X;
	}
	
	//카메라의 현재 회전에 맞춰서, Move Direction을 변환 
	MoveDirection = FTransform(GetControlRotation()).TransformVector(MoveDirection);

	AddMovementInput(MoveDirection);

	MoveDirection = FVector::ZeroVector;
}

void APuckSlayer::LookUp(const FInputActionValue& Value)
{
	float _CurrentValue;
	if(IsInvertLookUp)
	{
		_CurrentValue = Value.Get<float>() * -1;
	}
	else
	{
		_CurrentValue = Value.Get<float>();
	}
	AddControllerPitchInput(_CurrentValue / 4);
}

void APuckSlayer::Turn(const FInputActionValue& Value)
{
	AddControllerYawInput(Value.Get<float>()/4);
}

void APuckSlayer::_Jump(const FInputActionValue& Value)
{
	Jump();
}
