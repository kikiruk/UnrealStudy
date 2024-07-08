// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NavigationSystem.h"

AMyAIController::AMyAIController() : BehaviorTreeAsset (nullptr), BlackboardComponent (nullptr)
{
	// ������ ������Ʈ�� �ʱ�ȭ�Ѵ�.
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));

    // �����̺�� Ʈ�� ������ ã��, �ε��Ѵ�.
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
        // �����带 �ʱ�ȭ�Ѵ�
        if (UseBlackboard(BehaviorTreeAsset->BlackboardAsset, BlackboardComponent))
        {
            // �����̺��Ʈ���� �����Ѵ�
            RunBehaviorTree(BehaviorTreeAsset);
            UE_LOG(LogTemp, Warning, TEXT("BehaviorTree started: %s"), *BehaviorTreeAsset->GetName());
        }
    }
}

void AMyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds); // �ݵ�� ȣ���ؾ� �մϴ�
}
