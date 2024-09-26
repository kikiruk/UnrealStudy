// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_EnemyMoveTo.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
//#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
//#include "GameFramework/Pawn.h"

UBTTask_EnemyMoveTo::UBTTask_EnemyMoveTo()
{
    NodeName = "Enemy Move To Location"; // 노드 이름을 "Move To Random Location"으로 설정
    bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_EnemyMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    return EBTNodeResult::InProgress;
}

void UBTTask_EnemyMoveTo::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    // AIController 가져오기
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    // 블랙보드 가져오기
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

    //블랙보드 키값 가져오기 
    FVector TargetLocation = BlackboardComp->GetValueAsVector(FName("TargetLocationKey"));
    // 이전 위치와 현재 위치 비교
    if (!TargetLocation.Equals(LastTargetLocation, 1.0f))  // 1.0f는 허용 오차
    {
        // 위치가 변경되었으므로 MoveTo 호출
        FNavPathSharedPtr NavPath;
        FAIMoveRequest MoveRequest;
        MoveRequest.SetGoalLocation(TargetLocation);
        MoveRequest.SetAcceptanceRadius(50.0f);  // AI가 목표에 도착했다고 판단할 거리

        AIController->MoveTo(MoveRequest, &NavPath);
        // LastTargetLocation을 갱신
        LastTargetLocation = TargetLocation;

        // 경로를 성공적으로 찾았는지 로그 출력 (디버깅용)
        if (NavPath.IsValid())
        {
            UE_LOG(LogTemp, Warning, TEXT("Moving to new target location!"));
        }
    }
    
    // 목표 위치로의 방향 벡터 계산
    FVector Direction = (TargetLocation - AIPawn->GetActorLocation()).GetSafeNormal();

    // 목표 회전 (목표 위치를 바라보도록 설정)
    FRotator TargetRotation = Direction.Rotation();

    // 부드럽게 회전 (속도 조절: InterpSpeed 값으로 설정)
    float InterpSpeed = 8.f;  // 회전 속도 설정
    FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaSeconds, InterpSpeed);
    CurrentRotation = NewRotation;// 현재 회전 저장

    // 새 회전을 설정
    AIPawn->SetActorRotation(NewRotation);

    // 목표 위치에 도달하면 작업 완료 처리
    if (AIController->GetPawn()->GetActorLocation().Equals(TargetLocation, 100.0f))
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}
