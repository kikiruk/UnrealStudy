// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/EnemyAIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NavigationSystem.h"
#include "Actors/Characters/BaseCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

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
            // ������ Ű �̸��� �������� ����
            IsDieKey.SelectedKeyName = "IsDie";
            IsDieKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(AEnemyAIController, IsDieKey));// ������ Ű�� �ʱ�ȭ�ϰ� ���͸� �߰��մϴ�.
            BlackboardComponent->ClearValue(IsDieKey.SelectedKeyName); //�������� isDie ��� Ű NotSet ���� �ʱ�ȭ

            bool bIsDieValue = BlackboardComponent->GetValueAsBool(IsDieKey.SelectedKeyName);
            
            RunBehaviorTree(BehaviorTreeAsset); // �����̺��Ʈ���� �����Ѵ�
        }
    }

    ACharacter* controlledCharacter = GetCharacter();
    if (controlledCharacter)
    {
        ABaseCharacter* controlledBaseCharacter = Cast<ABaseCharacter>(controlledCharacter);
        if (controlledBaseCharacter) //Character �� �׾����� �̺�Ʈ�� ���
        {
            controlledBaseCharacter->BaseCharacterOnDeath.AddDynamic(this, &AEnemyAIController::OwnerCharacerDeath);
        }
    }
}

void AEnemyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds); // �ݵ�� ȣ���ؾ� �մϴ�
}

void AEnemyAIController::OwnerCharacerDeath()
{
    if (BehaviorTreeAsset)
    {
        BlackboardComponent->SetValueAsBool(IsDieKey.SelectedKeyName, true);
    }
}
