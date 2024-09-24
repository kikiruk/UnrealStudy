// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAnimInstance.h"
#include "EnemyAnimInstance.generated.h"

// ��������Ʈ ����
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

/*********************��Ÿ�� ����***************************/
	void Play_AttackMontage_ComboOne() { Montage_Play(AttackMontage_one, 1.5f); };
	void Play_AttackMontage_ComboTwo() { Montage_Play(AttackMontage_two, 1.5f); };
	void Play_AttackMontage_ComboThree() { Montage_Play(AttackMontage_three, 1.5f); };

/*************************��������Ʈ****************************/
public:
	FOnMontageNotifyReceived OnMontageNotifyReceived;

protected:
/****************��Ÿ��*******************/
	UPROPERTY(Category = "Animation", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UAnimMontage* AttackMontage_one;

	UPROPERTY(Category = "Animation", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UAnimMontage* AttackMontage_two;

	UPROPERTY(Category = "Animation", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UAnimMontage* AttackMontage_three;

	UPROPERTY(Category = "Animation", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UAnimMontage* LeveLStartMontage;

/*******************���� ����************************/

/*******************����� ���� �Լ�*******************/
	virtual void OnStatesChanged() override;

protected:
};
