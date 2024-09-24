// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAnimInstance.h"
#include "EnemyAnimInstance.generated.h"

// 델리게이트 선언
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMontageNotifyReceived, FName);

/**
 * 
 */
UCLASS()
class BASICUNREALPROJECT_API UEnemyAnimInstance : public UBaseAnimInstance
{
	GENERATED_BODY()

public:
	UEnemyAnimInstance();

public:
	virtual void NativeBeginPlay() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION()
	void AnimNotify_SaveAttack();

	UFUNCTION()
	void AnimNotify_ResetCombo();

	UFUNCTION()
	void OnLevelStartMontageEnded(UAnimMontage* Montage, bool bInterrupted);

/*********************몽타주 실행***************************/
	void Play_AttackMontage_ComboOne() { Montage_Play(AttackMontage_one, 1.5f); };
	void Play_AttackMontage_ComboTwo() { Montage_Play(AttackMontage_two, 1.5f); };
	void Play_AttackMontage_ComboThree() { Montage_Play(AttackMontage_three, 1.5f); };

/*************************델리게이트****************************/
public:
	FOnMontageNotifyReceived OnMontageNotifyReceived;

protected:
/****************몽타주*******************/
	UPROPERTY(Category = "Animation", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UAnimMontage* AttackMontage_one;

	UPROPERTY(Category = "Animation", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UAnimMontage* AttackMontage_two;

	UPROPERTY(Category = "Animation", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UAnimMontage* AttackMontage_three;

	UPROPERTY(Category = "Animation", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UAnimMontage* LeveLStartMontage;

/*******************상태 변수************************/

/*******************사용자 정의 함수*******************/
	virtual void OnStatesChanged() override;

protected:
};
