// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_Attack.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "EnemyAnimInstance.h"

UMyBTTask_Attack::UMyBTTask_Attack()
{
    NodeName = "Enemy Attack";
}

EBTNodeResult::Type UMyBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // AIController와 Pawn 가져오기
    AAIController* AIController = OwnerComp.GetAIOwner();
    APawn* ControlledPawn = AIController ? AIController->GetPawn() : nullptr;

    ACharacter* Character = Cast<ACharacter>(ControlledPawn);
    UE_LOG(LogTemp, Log, TEXT("Prepair to attack!"));
    if (Character)
    {
        // Skeletal Mesh Component 가져오기
        USkeletalMeshComponent* SkeletalMeshComponent = Character->GetMesh();
        if (SkeletalMeshComponent)
        {
            // AnimInstance 가져오기
            UAnimInstance* Instance = SkeletalMeshComponent->GetAnimInstance();

            UEnemyAnimInstance* AnimInstance = Cast<UEnemyAnimInstance>(Instance);
            if (AnimInstance)
            {
                AnimInstance->AnimateAttackMontage(); // 사용자 정의 함수 AnimateAttackMontage()
            }
        }
    }

	return EBTNodeResult::Type();
}
