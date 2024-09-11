// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenuButton.h"
#include "Components/CanvasPanelSlot.h"

void UMainMenuButton::SynchronizeProperties()
{
	Super::SynchronizeProperties();

    // 위젯이 CanvasPanel에 들어있는지 확인
    if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Slot))
    {
        // SizeToContent 옵션을 활성화
        CanvasSlot->SetAutoSize(true);
    }

    // 버튼의 배경 브러시를 제거하여 투명하게 설정
    WidgetStyle.Normal.SetResourceObject(nullptr);  // Normal 상태의 브러시 제거
    WidgetStyle.Hovered.SetResourceObject(nullptr); // Hovered 상태의 브러시 제거
    WidgetStyle.Pressed.SetResourceObject(nullptr); // Pressed 상태의 브러시 제거

    // 투명한 버튼을 클릭할 수 있도록 동작을 유지
    WidgetStyle.Normal.TintColor = FSlateColor(FLinearColor(0, 0, 0, 0));  // 투명 배경
    //WidgetStyle.Hovered.TintColor = FSlateColor(FLinearColor(0, 0, 0, 0)); // 투명 배경
    WidgetStyle.Pressed.TintColor = FSlateColor(FLinearColor(0, 0, 0, 0)); // 투명 배경

}
