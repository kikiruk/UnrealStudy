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
    // AIController�� Pawn ��������
    AAIController* AIController = OwnerComp.GetAIOwner();
    APawn* ControlledPawn = AIController ? AIController->GetPawn() : nullptr;

    ACharacter* Character = Cast<ACharacter>(ControlledPawn);
    UE_LOG(LogTemp, Log, TEXT("Prepair to attack!"));
    if (Character)
    {
        // Skeletal Mesh Component ��������
        USkeletalMeshComponent* SkeletalMeshComponent = Character->GetMesh();
        if (SkeletalMeshComponent)
        {
            // AnimInstance ��������
            UAnimInstance* Instance = SkeletalMeshComponent->GetAnimInstance();

            UEnemyAnimInstance* AnimInstance = Cast<UEnemyAnimInstance>(Instance);
            if (AnimInstance)
            {
                AnimInstance->AnimateAttackMontage(); // ����� ���� �Լ� AnimateAttackMontage()
            }
        }
    }

	return EBTNodeResult::Type();
}
