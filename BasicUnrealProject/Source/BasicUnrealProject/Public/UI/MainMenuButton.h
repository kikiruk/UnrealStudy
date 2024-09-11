// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "MainMenuButton.generated.h"

/**
 * 
 */
UCLASS()
class BASICUNREALPROJECT_API UMainMenuButton : public UButton
{
	GENERATED_BODY()

public:
	virtual void SynchronizeProperties() override;

};
