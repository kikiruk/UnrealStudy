// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_EnemyMoveTo.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
//#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
//#include "GameFramework/Pawn.h"

UBTTask_EnemyMoveTo::UBTTask_EnemyMoveTo()
{
    NodeName = "Enemy Move To Location"; // ��� �̸��� "Move To Random Location"���� ����
    bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_EnemyMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    return EBTNodeResult::InProgress;
}

void UBTTask_EnemyMoveTo::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    // AIController ��������
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    // ������ ��������
    UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
    if (!BlackboardComp)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    APawn* AIPawn = AIController->GetPawn();
    if (!AIPawn)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    //������ Ű�� �������� 
    FVector TargetLocation = BlackboardComp->GetValueAsVector(FName("TargetLocationKey"));
    // ���� ��ġ�� ���� ��ġ ��
    if (!TargetLocation.Equals(LastTargetLocation, 1.0f))  // 1.0f�� ��� ����
    {
        // ��ġ�� ����Ǿ����Ƿ� MoveTo ȣ��
        FNavPathSharedPtr NavPath;
        FAIMoveRequest MoveRequest;
        MoveRequest.SetGoalLocation(TargetLocation);
        MoveRequest.SetAcceptanceRadius(50.0f);  // AI�� ��ǥ�� �����ߴٰ� �Ǵ��� �Ÿ�

        AIController->MoveTo(MoveRequest, &NavPath);
        // LastTargetLocation�� ����
        LastTargetLocation = TargetLocation;

        // ��θ� ���������� ã�Ҵ��� �α� ��� (������)
        if (NavPath.IsValid())
        {
            UE_LOG(LogTemp, Warning, TEXT("Moving to new target location!"));
        }
    }
    
    // ��ǥ ��ġ���� ���� ���� ���
    FVector Direction = (TargetLocation - AIPawn->GetActorLocation()).GetSafeNormal();

    // ��ǥ ȸ�� (��ǥ ��ġ�� �ٶ󺸵��� ����)
    FRotator TargetRotation = Direction.Rotation();

    // �ε巴�� ȸ�� (�ӵ� ����: InterpSpeed ������ ����)
    float InterpSpeed = 8.f;  // ȸ�� �ӵ� ����
    FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaSeconds, InterpSpeed);
    CurrentRotation = NewRotation;// ���� ȸ�� ����

    // �� ȸ���� ����
    AIPawn->SetActorRotation(NewRotation);

    // ��ǥ ��ġ�� �����ϸ� �۾� �Ϸ� ó��
    if (AIController->GetPawn()->GetActorLocation().Equals(TargetLocation, 100.0f))
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}
