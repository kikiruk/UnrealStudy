// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class BASICUNREALPROJECT_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UEnemyAnimInstance();

public:
	virtual void NativeBeginPlay() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void AnimateAttackMontage();
private:
	UPROPERTY(Category = "Animation", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UAnimMontage* MyAttackMontage;
};
