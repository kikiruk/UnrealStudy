// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainMenuPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BASICUNREALPROJECT_API AMainMenuPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMainMenuPlayerController();

private:
	TSubclassOf<class UMainMenuWidget> MainMenuWidgetClass;

protected:

	virtual void BeginPlay() override;

};
