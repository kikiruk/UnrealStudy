// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenuButton.h"
#include "Components/CanvasPanelSlot.h"

void UMainMenuButton::SynchronizeProperties()
{
	Super::SynchronizeProperties();

    // ������ CanvasPanel�� ����ִ��� Ȯ��
    if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Slot))
    {
        // SizeToContent �ɼ��� Ȱ��ȭ
        CanvasSlot->SetAutoSize(true);
    }

    // ��ư�� ��� �귯�ø� �����Ͽ� �����ϰ� ����
    WidgetStyle.Normal.SetResourceObject(nullptr);  // Normal ������ �귯�� ����
    WidgetStyle.Hovered.SetResourceObject(nullptr); // Hovered ������ �귯�� ����
    WidgetStyle.Pressed.SetResourceObject(nullptr); // Pressed ������ �귯�� ����

    // ������ ��ư�� Ŭ���� �� �ֵ��� ������ ����
    WidgetStyle.Normal.TintColor = FSlateColor(FLinearColor(0, 0, 0, 0));  // ���� ���
    //WidgetStyle.Hovered.TintColor = FSlateColor(FLinearColor(0, 0, 0, 0)); // ���� ���
    WidgetStyle.Pressed.TintColor = FSlateColor(FLinearColor(0, 0, 0, 0)); // ���� ���

}
