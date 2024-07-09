// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class BASICUNREALPROJECT_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPlayerAnimInstance();

private:
	UPROPERTY(Category = "Animation", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true));
	float Speed;
	UPROPERTY(Category = "Animation", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true));
	float Horizontal;
	UPROPERTY(Category = "Animation", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true));
	float Vertical;
	UPROPERTY(Category = "Animation", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true));
	bool ShouldMove; //Idle <-> Walk �ִϸ��̼��� �����Ҷ��� ����
	UPROPERTY(Category = "Animation", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true));
	FRotator LookAtRotation; // ĳ���Ͱ� �ٶ󺸴� ������ ��Ÿ���� Rotator, �������Ʈ AimOffset ���� ���
	UPROPERTY(Category = "Animation", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UAnimMontage* MyFireMontage;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class APlayerCharacter* PlayerCharacter;

public:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeBeginPlay() override;

	//�������Ʈ�� Event Graph �� ���� ������ C++ ���� ������ �� �ִٰ� �����ְ� ��������
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	// ĳ������ �̺�Ʈ ���ε��� ���� �Լ�
	UFUNCTION(BlueprintCallable)
	void FireMontagePlay_BindToCharacterEvents();
};
