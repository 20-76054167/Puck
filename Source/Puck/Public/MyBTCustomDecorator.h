// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "MyBTCustomDecorator.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class PUCK_API UMyBTCustomDecorator : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UMyBTCustomDecorator();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

private:
	mutable bool bIsFirstExecution = true; // 처음 실행 여부 체크
};
