// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/EnemyAIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NavigationSystem.h"

AEnemyAIController::AEnemyAIController() : BehaviorTreeAsset (nullptr), BlackboardComponent (nullptr)
{
	// 블랙보드 컴포넌트를 초기화한다.
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));

    // 비헤이비어 트리 에셋을 찾고, 로드한다.
    static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTConstructor(TEXT("/Script/AIModule.BehaviorTree'/Game/MyBlueprint/BT_Enemy.BT_Enemy'"));

    if (BTConstructor.Succeeded())
    {
        BehaviorTreeAsset = BTConstructor.Object;
    }
}

void AEnemyAIController::BeginPlay()
{
    Super::BeginPlay();

    if (BehaviorTreeAsset)
    {
        // 블랙보드를 초기화한다
        if (UseBlackboard(BehaviorTreeAsset->BlackboardAsset, BlackboardComponent))
        {
            // 비헤이비어트리를 실행한다
            RunBehaviorTree(BehaviorTreeAsset);
        }
    }
}

void AEnemyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds); // 반드시 호출해야 합니다
}
