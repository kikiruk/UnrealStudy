// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/EnemyAnimInstance.h"
#include "Actors/Characters/EnemyCharacter.h"
#include "Controllers/EnemyAIController.h"
#include "GameFramework/PawnMovementComponent.h"

UEnemyAnimInstance::UEnemyAnimInstance()
{
	/********************************************캐릭터 기본 공격 애니메이션 **************************************************/
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

	/************************************************게임 시작 애니메이션***************************************************/
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

	// 델리게이트 설정
	FOnMontageEnded MontageEndedDelegate;
	MontageEndedDelegate.BindUObject(this, &UEnemyAnimInstance::OnLevelStartMontageEnded); // OnMontageEnded는 콜백 함수
	Montage_SetEndDelegate(MontageEndedDelegate, LeveLStartMontage);
}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	//애니메이션에 사용될 변수값들 멤버 변수 선언할 필요 있음, 이부분에서 세팅
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




