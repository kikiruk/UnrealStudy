// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include "MyEnemy.h"

UEnemyAnimInstance::UEnemyAnimInstance()
	: PawnOwnedEnemy(nullptr), MyAttackMontage(nullptr)
{
	ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMontage(TEXT(
		"/Script/Engine.AnimMontage'/Game/MyBlueprint/Animation/Enemy/AM_Attack.AM_Attack'"));

	if (AttackMontage.Succeeded())
	{
		MyAttackMontage = AttackMontage.Object;
	}

}

void UEnemyAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	// ĳ���� �ν��Ͻ��� ������
	PawnOwnedEnemy = Cast<AMyEnemy>(TryGetPawnOwner());
}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	//�ִϸ��̼ǿ� ���� �������� ��� ���� ������ �ʿ� ����, �̺κп��� ����
}

void UEnemyAnimInstance::AnimateAttackMontage()
{
	if (MyAttackMontage && !Montage_IsPlaying(MyAttackMontage))
	{
		UE_LOG(LogTemp, Log, TEXT("ATTACK MONTAGE INITIALIZED4444444444444444"));
		Montage_Play(MyAttackMontage);
	}
}

