// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAnimInstance.h"
#include "EnemyAnimInstance.generated.h"

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
	virtual bool TryPlayAttackMontage();

	UFUNCTION()
	void OnLevelStartMontageEnded(UAnimMontage* Montage, bool bInterrupted);

protected:
	UPROPERTY(Category = "Animation", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UAnimMontage* AttackMontage_one;

	UPROPERTY(Category = "Animation", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UAnimMontage* AttackMontage_two;

	UPROPERTY(Category = "Animation", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UAnimMontage* AttackMontage_three;

	UPROPERTY(Category = "Animation", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UAnimMontage* LeveLStartMontage;

protected:
	bool SaveAttack; // 다음 공격 콤보가 준비된 상태이면 true
	int ComboNum; // 몇번째 콤보인지 표시
};
