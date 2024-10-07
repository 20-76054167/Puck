// Fill out your copyright notice in the Description page of Project Settings.


#include "NotifyState_EnemyAttack.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"

void UNotifyState_EnemyAttack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Notify Begin"));
	//Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	normalEnemy = Cast<ANormalEnemy>(MeshComp->GetOwner());

	if (normalEnemy)
	{
		normalEnemy->AttackPlayer();
	}

}

void UNotifyState_EnemyAttack::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Notify Tick"));
	//Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	
	

}

void UNotifyState_EnemyAttack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Notify End"));

	//Super::NotifyEnd(MeshComp, Animation, EventReference);

	
}
