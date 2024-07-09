// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_UpdateTargetLocation.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_UpdateTargetLocation::UBTService_UpdateTargetLocation()
{
    NodeName = "Update Target Location";    // ����� �̸��� "Update Target Location"���� �����մϴ�.
    Interval = 0.2f;               // ���񽺰� ����Ǵ� ������ 0.2�ʷ� �����մϴ�.

    // ������ Ű�� �����մϴ�. (TargetLocationKey)
    TargetLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_UpdateTargetLocation, TargetLocationKey)); 
    
    // ������ Ű�� �����մϴ�. (Target) ���⼭ AActor::StaticClass() �� �ص� ���� �۵� �Ǿ���ϳ�, ���� �ʾƼ� UObject::StaticClass() �� �ϴϱ� �ߵ�
    TargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_UpdateTargetLocation, TargetKey), UObject::StaticClass());
}

void UBTService_UpdateTargetLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (BlackboardComp)
    {
        // ������ Ű�� �����ͼ� ���
        UObject* TargetObject = BlackboardComp->GetValueAsObject(TargetKey.SelectedKeyName);
        if (!TargetObject) return;

        APawn* TargetPawn = Cast<APawn>(TargetObject);
        if (!TargetPawn) return;

        BlackboardComp->SetValueAsVector(TargetLocationKey.SelectedKeyName, TargetPawn->GetActorLocation());
    }
}
