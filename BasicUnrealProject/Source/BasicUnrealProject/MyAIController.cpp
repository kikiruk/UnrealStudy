// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NavigationSystem.h"

AMyAIController::AMyAIController() : BehaviorTreeAsset (nullptr), BlackboardComponent (nullptr)
{
	// 블랙보드 컴포넌트를 초기화한다.
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));

    // 비헤이비어 트리 에셋을 찾고, 로드한다.
    static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/MyBlueprint/BT_MyBehaviorTree.BT_MyBehaviorTree'"));
    if (BTObject.Succeeded())
    {
        BehaviorTreeAsset = BTObject.Object;
        UE_LOG(LogTemp, Warning, TEXT("BehaviorTree started: %s"), *BehaviorTreeAsset->GetName());
    }
}

void AMyAIController::BeginPlay()
{
    Super::BeginPlay();

    if (BehaviorTreeAsset)
    {
        // 블랙보드를 초기화한다
        if (UseBlackboard(BehaviorTreeAsset->BlackboardAsset, BlackboardComponent))
        {
            // 비헤이비어트리를 실행한다
            RunBehaviorTree(BehaviorTreeAsset);
            UE_LOG(LogTemp, Warning, TEXT("BehaviorTree started: %s"), *BehaviorTreeAsset->GetName());
        }
    }
}

void AMyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds); // 반드시 호출해야 합니다
}
