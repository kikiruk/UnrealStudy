// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/EnemyAIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NavigationSystem.h"

AEnemyAIController::AEnemyAIController() : BehaviorTreeAsset (nullptr), BlackboardComponent (nullptr)
{
	// ������ ������Ʈ�� �ʱ�ȭ�Ѵ�.
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));

    // �����̺�� Ʈ�� ������ ã��, �ε��Ѵ�.
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
        // �����带 �ʱ�ȭ�Ѵ�
        if (UseBlackboard(BehaviorTreeAsset->BlackboardAsset, BlackboardComponent))
        {
            // �����̺��Ʈ���� �����Ѵ�
            RunBehaviorTree(BehaviorTreeAsset);
        }
    }
}

void AEnemyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds); // �ݵ�� ȣ���ؾ� �մϴ�
}
