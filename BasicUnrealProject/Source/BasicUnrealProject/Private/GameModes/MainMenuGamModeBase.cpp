// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/MainMenuGamModeBase.h"
#include "Blueprint/UserWidget.h"
#include "UI/MainMenuWidget.h"
#include "Controllers/MainMenuPlayerController.h"

AMainMenuGamModeBase::AMainMenuGamModeBase()
{
	PlayerControllerClass = AMainMenuPlayerController::StaticClass();
}

void AMainMenuGamModeBase::BeginPlay()
{
}
