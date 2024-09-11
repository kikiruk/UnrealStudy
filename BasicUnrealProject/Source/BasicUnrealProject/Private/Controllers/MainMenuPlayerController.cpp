// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/MainMenuPlayerController.h"
#include "UI/MainMenuWidget.h"

AMainMenuPlayerController::AMainMenuPlayerController()
{
    ConstructorHelpers::FClassFinder<UMainMenuWidget> MainMenuWidgetFinder(
        TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/MyBlueprint/BP_MainMenuUserWidget.BP_MainMenuUserWidget_C'"));

    if (MainMenuWidgetFinder.Succeeded())
    {
        MainMenuWidgetClass = MainMenuWidgetFinder.Class;
    }
}

void AMainMenuPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (MainMenuWidgetClass)
    {
        UMainMenuWidget* MainMenu = CreateWidget<UMainMenuWidget>(this, MainMenuWidgetClass);
        if (MainMenu)
        {
            MainMenu->AddToViewport();
            SetInputMode(FInputModeUIOnly());
            bShowMouseCursor = true;
        }
    }
}
