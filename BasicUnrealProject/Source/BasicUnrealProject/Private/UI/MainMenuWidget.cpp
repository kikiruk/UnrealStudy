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
        // StartGameButton 클릭 이벤트 연결
        FScriptDelegate StartGameDelegate;
        StartGameDelegate.BindUFunction(this, "OnStartGameClicked");
        StartGameButton->OnClicked.Add(StartGameDelegate);
    }
        
    // QuitGameButton 클릭 이벤트 연결
    if (QuitGameButton)
    {
        FScriptDelegate QuitGameDelegate;
        QuitGameDelegate.BindUFunction(this, "OnQuitGameClicked");
        QuitGameButton->OnClicked.Add(QuitGameDelegate);
    }
}

void UMainMenuWidget::OnStartGameClicked()
{
	// 게임 레벨 변경
	UGameplayStatics::OpenLevel(this, FName("Level_One"));
}

void UMainMenuWidget::OnQuitGameClicked()
{
    //게임종료
    UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, true);
}


