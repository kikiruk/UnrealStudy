// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class BASICUNREALPROJECT_API UPlayerAnimInstance : public UBaseAnimInstance
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

	// PlayerCharacter를 멤버 변수로 저장하여 NativeUpdateAnimation에서 반복적으로 캐스팅하지 않도록 합니다.
	// 게임 플레이 중 캐릭터가 변경될 수 있으므로 필요 시 이 변수를 초기화하거나 갱신해야 합니다.
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
