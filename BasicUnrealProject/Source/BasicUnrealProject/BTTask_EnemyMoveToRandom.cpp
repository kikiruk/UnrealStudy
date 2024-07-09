#include "BTTask_EnemyMoveToRandom.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "Runtime/NavigationSystem/Public/NavigationPath.h"
#include "Navigation/PathFollowingComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.h"

// ������: ��� �̸��� �⺻ Ž�� �ݰ� ����
UBTTask_EnemyMoveToRandom::UBTTask_EnemyMoveToRandom()
{
    NodeName = "Enemy Move To Random Location"; // ��� �̸��� "Move To Random Location"���� ����
    bNotifyTick = true;
    SearchRadius = 1000.0f; // �⺻ Ž�� �ݰ��� 1000.0f�� ����
}

// �½�ũ ���� �Լ�
EBTNodeResult::Type UBTTask_EnemyMoveToRandom::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // AIController�� Pawn ��������
    AAIController* AIController = OwnerComp.GetAIOwner();
    APawn* ControlledPawn = AIController ? AIController->GetPawn() : nullptr;

    // Pawn�� ���� ��� ���� ����
    if (ControlledPawn == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    // �׺���̼� �ý��� ��������
    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
    // �׺���̼� �ý����� ���� ��� ���� ����
    if (NavSys == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    // ���� Pawn�� ��ġ ��������
    FVector Origin = ControlledPawn->GetActorLocation();
    FNavLocation NavLocation;

    // �־��� �ݰ� ������ ������ Ž�� ������ ��ġ ã��
    if (NavSys->GetRandomPointInNavigableRadius(Origin, SearchRadius, NavLocation))
    {
        FVector RandomLocation = NavLocation.Location; // ã�� ��ġ ����

        // �̵� ��û ����
        FAIMoveRequest MoveRequest;
        MoveRequest.SetGoalLocation(RandomLocation); // ��ǥ ��ġ ����
        MoveRequest.SetAcceptanceRadius(5.0f); // ��ǥ ��ġ�� ���� ��� �ݰ� ����

        // �׺���̼� ��� ������ ����
        FNavPathSharedPtr NavPath;
        // �̵� ��� ���� �� ��� ����
        EPathFollowingRequestResult::Type MoveResult = AIController->MoveTo(MoveRequest, &NavPath);

        // �̵� ��û�� �������� ��� ���� ����
        if (MoveResult == EPathFollowingRequestResult::RequestSuccessful)
        {
            return EBTNodeResult::InProgress;
        }
    }

    // �̵� ��û�� ������ ��� ���� ����
    UE_LOG(LogTemp, Log, TEXT("AI Task Status: Failed"));
    return EBTNodeResult::Failed;
}


void UBTTask_EnemyMoveToRandom::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    // AIController ��������
    AAIController* AIController = OwnerComp.GetAIOwner();

    // Pawn�� ���� ��� ���� ����
    if (AIController == nullptr)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
    }
    else
    {
        // AIController�� Pawn ��������
        EPathFollowingStatus::Type moveStatus = AIController->GetMoveStatus();
    
        if (moveStatus == EPathFollowingStatus::Idle)
        {
            FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        }

        const TCHAR* StatusString =
            (moveStatus == EPathFollowingStatus::Idle) ? TEXT("Idle") :
            (moveStatus == EPathFollowingStatus::Waiting) ? TEXT("Waiting") :
            (moveStatus == EPathFollowingStatus::Paused) ? TEXT("Paused") :
            (moveStatus == EPathFollowingStatus::Moving) ? TEXT("Moving") :
            TEXT("Unknown");

        UE_LOG(LogTemp, Log, TEXT("AI Movement Status: %s"), StatusString);
    }
}
