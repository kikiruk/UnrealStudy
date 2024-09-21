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
	

protected:
	UFUNCTION(BlueprintCallable)
	bool GetIsIdle();
};
