// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include "EnemyCharacter.h"

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

	// 캐릭터 인스턴스를 가져옴
	PawnOwnedEnemy = Cast<AEnemyCharacter>(TryGetPawnOwner());
}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	//애니메이션에 사용될 변수값들 멤버 변수 선언할 필요 있음, 이부분에서 세팅
}

void UEnemyAnimInstance::AnimateAttackMontage()
{
	if (MyAttackMontage && !Montage_IsPlaying(MyAttackMontage))
	{
		UE_LOG(LogTemp, Log, TEXT("Attack!"));
		Montage_Play(MyAttackMontage);
	}
}

