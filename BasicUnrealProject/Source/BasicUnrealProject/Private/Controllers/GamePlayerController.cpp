// Fill out your copyright notice in the Description page of Project Settings.

#include "Controllers/GamePlayerController.h"

AGamePlayerController::AGamePlayerController()
{
	
}

void AGamePlayerController::BeginPlay()
{
	bShowMouseCursor = false;
	SetInputMode(FInputModeGameOnly());
}
