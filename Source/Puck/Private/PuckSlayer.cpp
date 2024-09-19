// Fill out your copyright notice in the Description page of Project Settings.


#include "PuckSlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "PLauncher.h"
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
	springArmComp->TargetArmLength = 200;
	springArmComp->bUsePawnControlRotation = true;


	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	cameraComp->SetupAttachment(springArmComp);
	cameraComp->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = true;

	//WeaponMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	//WeaponMeshComp->SetupAttachment(GetMesh());

	FName WeaponSocket(TEXT("hand_rSocket"));
	if (GetMesh()->DoesSocketExist(WeaponSocket))
	{
		LauncherWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WEAPON"));

		static ConstructorHelpers::FObjectFinder<UStaticMesh> SW(TEXT("/Script/Engine.StaticMesh'/Game/Blueprints/HugeWeapon.HugeWeapon'"));
		if (SW.Succeeded())
		{
			LauncherWeapon->SetStaticMesh(SW.Object);
		}
		LauncherWeapon->SetupAttachment(GetMesh(), WeaponSocket);

	}
//	PLauncher = GetWorld()->SpawnActor<APLauncher>(APLauncher::StaticClass());
	//PLauncher->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("hand_r"));


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
