// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/MainMenuButton.h"
#include "Components/Image.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

    if (StartGameButton)
    {
        // StartGameButton Ŭ�� �̺�Ʈ ����
        FScriptDelegate StartGameDelegate;
        StartGameDelegate.BindUFunction(this, "OnStartGameClicked");
        StartGameButton->OnClicked.Add(StartGameDelegate);
    }
        
    // QuitGameButton Ŭ�� �̺�Ʈ ����
    if (QuitGameButton)
    {
        FScriptDelegate QuitGameDelegate;
        QuitGameDelegate.BindUFunction(this, "OnQuitGameClicked");
        QuitGameButton->OnClicked.Add(QuitGameDelegate);
    }
}

void UMainMenuWidget::OnStartGameClicked()
{
	// ���� ���� ����
	UGameplayStatics::OpenLevel(this, FName("Level_One"));
}

void UMainMenuWidget::OnQuitGameClicked()
{
    //��������
    UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, true);
}


