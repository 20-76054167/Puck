// Fill out your copyright notice in the Description page of Project Settings.


#include "FireActorComponent.h"

#include <string>

#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UFireActorComponent::UFireActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	if(GetOwner())
	{
		ownerPlayer = Cast<APuckSlayer>(GetOwner());
		if(ownerPlayer)
		{
			fireArrow = CreateDefaultSubobject<UArrowComponent>("Fire Arrow Component");
			//fireArrow->AttachToComponent(ownerPlayer->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("spine_01"));
			fireArrow->CreationMethod = EComponentCreationMethod::Instance;
			fireArrow->RegisterComponent();
			fireArrow->SetRelativeLocation(FVector(0, 50, 100));
			fireArrow->SetRelativeRotation(FRotator(0, 0, 90));
		}
	}

	ConstructorHelpers::FObjectFinder<UCurveFloat> curve(TEXT("/Script/Engine.CurveFloat'/Game/FireRecoilCurve.FireRecoilCurve'"));

	if(curve.Succeeded())
	{
		recoilCurve = curve.Object;
	}

	recoilTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline Component"));
	recoilStartCallback.BindUFunction(this, FName("RecoilStart"));
}


void UFireActorComponent::SetRecoilTimeline()
{
	if(recoilCurve)
	{
		recoilTimeline->AddInterpFloat(recoilCurve, recoilStartCallback);
		recoilTimeline->SetLooping(false);
	}
}

// Called when the game starts
void UFireActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	SetRecoilTimeline();

	AController* ownerController = ownerPlayer->GetController();
	if(ownerController)
	{
		playerController = Cast<APlayerController>(ownerController);
	}	
}

// Called every frame
void UFireActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	magazine = maxMagazine;
}

void UFireActorComponent::FireByTrace()
{
	if(!bCanAttack)
	{
		return;
	}

	ownerPlayer = Cast<APuckSlayer>(GetOwner());
	fireArrow = ownerPlayer->GetArrowComponent();
	
	for(int i=0; i < bulletNum; i++)
	{
		FVector _startLoc = fireArrow->GetComponentLocation();
		FVector _endLoc = _startLoc + fireArrow->GetForwardVector() * range;

		//FVector _startLoc = ownerPlayer->GetActorLocation();
		//FVector _endLoc = _startLoc + ownerPlayer->GetActorForwardVector() * range;

		FHitResult _hitRes;

		FCollisionQueryParams _collisionParam;
		_collisionParam.AddIgnoredActor(ownerPlayer);

		pitchRange = FMath::RandRange(-100, 100);
		rollRange = FMath::RandRange(-100, 100);
		yawRange = FMath::RandRange(-100, 100);

		_endLoc.X += pitchRange;
		_endLoc.Y += rollRange;
		_endLoc.Z += yawRange;
		
		bool isHit = GetWorld()->LineTraceSingleByChannel(_hitRes, _startLoc, _endLoc, ECC_Pawn, _collisionParam);
		DrawDebugLine(GetWorld(), _startLoc, _endLoc, FColor::Green, true, 5.f);
		
		if(isHit)
		{
			AActor* hitActor = _hitRes.GetActor();
			if(hitActor)
			{
				UGameplayStatics::ApplyDamage(hitActor, damage, ownerPlayer->GetController(), ownerPlayer, UDamageType::StaticClass());
			}
		}
	}
	
	recoilTimeline->PlayFromStart();
}

bool UFireActorComponent::Reload()
{
	if(magazine >= maxMagazine)
	{
		return false;
	}
	magazine++;
	
	return true;
}

/*void UFireActorComponent::AttackRecoil()
{
	//AController* ownerController = GetOwner()->GetInstigatorController();
	AController* ownerController = ownerPlayer->GetController();
	
	if(ownerController)
	{
		APlayerController* playerController = Cast<APlayerController>(ownerController);
		if(playerController)
		{
			playerController->AddPitchInput(-5.0f);
		}
	}
	
	this->RecoveryRecoil();
}*/

void UFireActorComponent::RecoilStart(float value)
{
	if(IsValid(playerController))
	{
		playerController->AddPitchInput(value);
	}
}

void UFireActorComponent::RecoveryRecoil()
{
	
}
