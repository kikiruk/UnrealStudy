// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_EnemyAttack.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "Actors/Characters/EnemyCharacter.h"

UBTTask_EnemyAttack::UBTTask_EnemyAttack()
{
    // TickTask �Լ��� ȣ��ǵ��� ����
    //bNotifyTick = true;

    NodeName = "Enemy Attack";
}

EBTNodeResult::Type UBTTask_EnemyAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // AIController�� Pawn ��������
    AAIController* AIController = OwnerComp.GetAIOwner();
    APawn* ControlledPawn = AIController ? AIController->GetPawn() : nullptr;
    AEnemyCharacter* enemyCharacter = Cast<AEnemyCharacter>(ControlledPawn);

    if (enemyCharacter)
    {
        enemyCharacter->Attack();
    }

	return EBTNodeResult::Succeeded;
}

//void UBTTask_EnemyAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
//{
//    // AIController�� Pawn ��������
//    AAIController* AIController = OwnerComp.GetAIOwner();
//    APawn* ControlledPawn = AIController ? AIController->GetPawn() : nullptr;
//    AEnemyCharacter* enemyCharacter = Cast<AEnemyCharacter>(ControlledPawn);
//
//    //if (enemyCharacter)
//    //{
//    //
//    //}
//}

