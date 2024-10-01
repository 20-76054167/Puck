// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTCustomDecorator.h"
#include "BehaviorTree/BlackboardComponent.h"

UMyBTCustomDecorator::UMyBTCustomDecorator()
{
    NodeName = "First Time Only"; // 데코레이터 이름
}

bool UMyBTCustomDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    // 블랙보드에서 bHasWaited 값을 확인
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (BlackboardComp && BlackboardComp->GetValueAsBool(TEXT("bHasWaited")) == false)
    {
        // 처음 실행이면 블랙보드에 true 설정
        BlackboardComp->SetValueAsBool(TEXT("bHasWaited"), true);
        return true;  // 처음에는 실행
    }

    return false; // 이후에는 조건을 만족하지 않음
}