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
	bool ShouldMove; //Idle <-> Walk 애니메이션을 변경할때의 조건
	UPROPERTY(Category = "Animation", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true));
	FRotator LookAtRotation; // 캐릭터가 바라보는 방향을 나타내는 Rotator, 블루프린트 AimOffset 에서 사용
	UPROPERTY(Category = "Animation", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UAnimMontage* MyFireMontage;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class APlayerCharacter* PlayerCharacter;

public:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeBeginPlay() override;

	//블루프린트의 Event Graph 와 같은 역할을 C++ 에서 수행할 수 있다고 곽상휘가 설명했음
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	// 캐릭터의 이벤트 바인딩을 위한 함수
	UFUNCTION(BlueprintCallable)
	void FireMontagePlay_BindToCharacterEvents();
};
