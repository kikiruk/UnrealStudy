// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/EnemyAnimInstance.h"

UEnemyAnimInstance::UEnemyAnimInstance()
{
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

	SaveAttack = false;	  // ���� ���� �޺��� �غ�� �����̸� true
	ComboNum = 1;		// ���° �޺����� ǥ��
}

void UEnemyAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	//�ִϸ��̼ǿ� ���� �������� ��� ���� ������ �ʿ� ����, �̺κп��� ����
}

void UEnemyAnimInstance::AnimNotify_SaveAttack()
{
	SaveAttack = true;
}

void UEnemyAnimInstance::AnimNotify_ResetCombo()
{
	SaveAttack = false;
	ComboNum = 1;
}

bool UEnemyAnimInstance::TryPlayAttackMontage()
{
	if (SaveAttack == true || ComboNum == 1)
	{
		SaveAttack = false;

		UE_LOG(LogTemp, Error, TEXT("ComboNum : %d"), ComboNum);

		if (!AttackMontage_one || !AttackMontage_two || !AttackMontage_three)
		{
			UE_LOG(LogTemp, Error, TEXT("Montage is not properly set!"));
		}

		if (ComboNum == 1) {
			Montage_Play(AttackMontage_one);
		}
		else if (ComboNum == 2)
		{
			Montage_Play(AttackMontage_two);
		}
		else if (ComboNum == 3)
		{
			Montage_Play(AttackMontage_three);
		}

		ComboNum++;
		return true;
	}
	else return false;
}

