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
            // 블랙보드 키 이름을 수동으로 설정
            IsDieKey.SelectedKeyName = "IsDie";
            IsDieKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(AEnemyAIController, IsDieKey));// 블랙보드 키를 초기화하고 필터를 추가합니다.
            BlackboardComponent->ClearValue(IsDieKey.SelectedKeyName); //블랙보드의 isDie 라는 키 NotSet 으로 초기화

            bool bIsDieValue = BlackboardComponent->GetValueAsBool(IsDieKey.SelectedKeyName);
            
            RunBehaviorTree(BehaviorTreeAsset); // 비헤이비어트리를 실행한다
        }
    }

    ACharacter* controlledCharacter = GetCharacter();
    if (controlledCharacter)
    {
        ABaseCharacter* controlledBaseCharacter = Cast<ABaseCharacter>(controlledCharacter);
        if (controlledBaseCharacter) //Character 가 죽었을때 이벤트를 등록
        {
            controlledBaseCharacter->BaseCharacterOnDeath.AddDynamic(this, &AEnemyAIController::OwnerCharacerDeath);
        }
    }
}

void AEnemyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds); // 반드시 호출해야 합니다
}

void AEnemyAIController::OwnerCharacerDeath()
{
    if (BehaviorTreeAsset)
    {
        BlackboardComponent->SetValueAsBool(IsDieKey.SelectedKeyName, true);
    }
}
