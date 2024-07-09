// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_UpdateTargetLocation.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_UpdateTargetLocation::UBTService_UpdateTargetLocation()
{
    NodeName = "Update Target Location";    // 노드의 이름을 "Update Target Location"으로 설정합니다.
    Interval = 0.2f;               // 서비스가 실행되는 간격을 0.2초로 설정합니다.

    // 블랙보드 키를 설정합니다. (TargetLocationKey)
    TargetLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_UpdateTargetLocation, TargetLocationKey)); 
    
    // 블랙보드 키를 설정합니다. (Target) 여기서 AActor::StaticClass() 를 해도 정상 작동 되어야하나, 되지 않아서 UObject::StaticClass() 로 하니까 잘됨
    TargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_UpdateTargetLocation, TargetKey), UObject::StaticClass());
}

void UBTService_UpdateTargetLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (BlackboardComp)
    {
        // 블랙보드 키를 가져와서 사용
        UObject* TargetObject = BlackboardComp->GetValueAsObject(TargetKey.SelectedKeyName);
        if (!TargetObject) return;

        APawn* TargetPawn = Cast<APawn>(TargetObject);
        if (!TargetPawn) return;

        BlackboardComp->SetValueAsVector(TargetLocationKey.SelectedKeyName, TargetPawn->GetActorLocation());
    }
}
