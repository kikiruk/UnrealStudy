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

// 생성자: 노드 이름과 기본 탐색 반경 설정
UBTTask_EnemyMoveToRandom::UBTTask_EnemyMoveToRandom()
{
    NodeName = "Enemy Move To Random Location"; // 노드 이름을 "Move To Random Location"으로 설정
    bNotifyTick = true;
    SearchRadius = 1000.0f; // 기본 탐색 반경을 1000.0f로 설정
}

// 태스크 실행 함수
EBTNodeResult::Type UBTTask_EnemyMoveToRandom::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // AIController와 Pawn 가져오기
    AAIController* AIController = OwnerComp.GetAIOwner();
    APawn* ControlledPawn = AIController ? AIController->GetPawn() : nullptr;

    // Pawn이 없는 경우 실패 리턴
    if (ControlledPawn == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    // 네비게이션 시스템 가져오기
    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
    // 네비게이션 시스템이 없는 경우 실패 리턴
    if (NavSys == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    // 현재 Pawn의 위치 가져오기
    FVector Origin = ControlledPawn->GetActorLocation();
    FNavLocation NavLocation;

    // 주어진 반경 내에서 임의의 탐색 가능한 위치 찾기
    if (NavSys->GetRandomPointInNavigableRadius(Origin, SearchRadius, NavLocation))
    {
        FVector RandomLocation = NavLocation.Location; // 찾은 위치 저장

        // 이동 요청 설정
        FAIMoveRequest MoveRequest;
        MoveRequest.SetGoalLocation(RandomLocation); // 목표 위치 설정
        MoveRequest.SetAcceptanceRadius(5.0f); // 목표 위치에 대한 허용 반경 설정

        // 네비게이션 경로 포인터 선언
        FNavPathSharedPtr NavPath;
        // 이동 명령 실행 및 결과 저장
        EPathFollowingRequestResult::Type MoveResult = AIController->MoveTo(MoveRequest, &NavPath);

        // 이동 요청이 성공적인 경우 성공 리턴
        if (MoveResult == EPathFollowingRequestResult::RequestSuccessful)
        {
            return EBTNodeResult::InProgress;
        }
    }

    // 이동 요청이 실패한 경우 실패 리턴
    UE_LOG(LogTemp, Log, TEXT("AI Task Status: Failed"));
    return EBTNodeResult::Failed;
}


void UBTTask_EnemyMoveToRandom::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    // AIController 가져오기
    AAIController* AIController = OwnerComp.GetAIOwner();

    // Pawn이 없는 경우 실패 리턴
    if (AIController == nullptr)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
    }
    else
    {
        // AIController와 Pawn 가져오기
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
