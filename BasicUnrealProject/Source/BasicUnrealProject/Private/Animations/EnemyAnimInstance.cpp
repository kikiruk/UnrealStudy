// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/EnemyAnimInstance.h"
#include "Actors/Characters/EnemyCharacter.h"
#include "Controllers/EnemyAIController.h"
#include "GameFramework/PawnMovementComponent.h"

UEnemyAnimInstance::UEnemyAnimInstance()
{
	/********************************************ĳ���� �⺻ ���� �ִϸ��̼� **************************************************/
	ConstructorHelpers::FObjectFinder<UAnimMontage> attackMontage_oneConstructor(
		TEXT("/Script/Engine.AnimMontage'/Game/ParagonGreystone/Characters/Heroes/Greystone/Animations/Attack_PrimaryA_Montage.Attack_PrimaryA_Montage'"));
	ConstructorHelpers::FObjectFinder<UAnimMontage> attackMontage_twoConstructor(
		TEXT("/Script/Engine.AnimMontage'/Game/ParagonGreystone/Characters/Heroes/Greystone/Animations/Attack_PrimaryB_Montage.Attack_PrimaryB_Montage'"));
	ConstructorHelpers::FObjectFinder<UAnimMontage> attackMontage_threeConstructor(
		TEXT("/Script/Engine.AnimMontage'/Game/ParagonGreystone/Characters/Heroes/Greystone/Animations/Attack_PrimaryC_Montage.Attack_PrimaryC_Montage'"));

	if (attackMontage_oneConstructor.Succeeded()) {
		AttackMontage_one = attackMontage_oneConstructor.Object;
	}

	if (attackMontage_twoConstructor.Succeeded()) {
		AttackMontage_two = attackMontage_twoConstructor.Object;
	}

	if (attackMontage_threeConstructor.Succeeded()) {
		AttackMontage_three = attackMontage_threeConstructor.Object;
	}

	/************************************************���� ���� �ִϸ��̼�***************************************************/
	ConstructorHelpers::FObjectFinder<UAnimMontage> LeveLStartMontageConstructor(
		TEXT("/Script/Engine.AnimMontage'/Game/ParagonGreystone/Characters/Heroes/Greystone/Animations/LevelStart_Montage.LevelStart_Montage'"));

	if (attackMontage_threeConstructor.Succeeded()) {
		LeveLStartMontage = LeveLStartMontageConstructor.Object;
	}
}

void UEnemyAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	Montage_Play(LeveLStartMontage, 1.5f);

	// ��������Ʈ ����
	FOnMontageEnded MontageEndedDelegate;
	MontageEndedDelegate.BindUObject(this, &UEnemyAnimInstance::OnLevelStartMontageEnded); // OnMontageEnded�� �ݹ� �Լ�
	Montage_SetEndDelegate(MontageEndedDelegate, LeveLStartMontage);
}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	//�ִϸ��̼ǿ� ���� �������� ��� ���� ������ �ʿ� ����, �̺κп��� ����
}

void UEnemyAnimInstance::AnimNotify_SaveAttack()
{
	OnMontageNotifyReceived.Broadcast(FName("SaveAttack"));
}

void UEnemyAnimInstance::AnimNotify_ResetCombo()
{
	OnMontageNotifyReceived.Broadcast(FName("ResetCombo"));
}

void UEnemyAnimInstance::AnimNotify_ApplyDamage()
{
	OwendBaseCharacter->ApplyDamage();
}

void UEnemyAnimInstance::OnLevelStartMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	UE_LOG(LogTemp, Warning, TEXT("UEnemyAnimInstance::OnLevelStartMontageEnded"));

	APawn* owendPawn = TryGetPawnOwner();
	if (!owendPawn) return;

	AEnemyCharacter* ownedEnemyCharacter = Cast<AEnemyCharacter>(owendPawn);
	if (!ownedEnemyCharacter) return;

	AController* controller = ownedEnemyCharacter->GetController();
	if (!controller) return;

	AEnemyAIController* enemyController = Cast<AEnemyAIController>(controller);
	if (!enemyController) return;

	enemyController->SetMontageEndedKeyTrue();
}

void UEnemyAnimInstance::OnStatesChanged()
{
	Super::OnStatesChanged();

	bool currentBIsIdle = OwendBaseCharacter->bIsIdle;
	if (bIsIdle != currentBIsIdle)
	{
		bIsIdle = currentBIsIdle;
		if (currentBIsIdle == false)
		{
			if (Montage_IsPlaying(AttackMontage_one)) Montage_Stop(0.2f, AttackMontage_one); 
			else if (Montage_IsPlaying(AttackMontage_two)) Montage_Stop(0.2f, AttackMontage_two);
			else if (Montage_IsPlaying(AttackMontage_three)) Montage_Stop(0.2f, AttackMontage_three);

		}
	}
}




