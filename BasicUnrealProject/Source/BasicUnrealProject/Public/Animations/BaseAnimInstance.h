// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class BASICUNREALPROJECT_API UBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeBeginPlay() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	class ABaseCharacter* OwendBaseCharacter;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character State")
	bool bIsIdle;

	virtual void OnStatesChanged() { return; };
};
