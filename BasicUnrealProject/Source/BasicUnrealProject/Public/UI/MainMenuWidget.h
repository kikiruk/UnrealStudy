// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class BASICUNREALPROJECT_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	// void AddToViewport(int32 ZOrder = 0); // �� �Լ��� UI ������ ȭ�鿡 �߰��ϴ� ������ �մϴ�. �Ϲ������� PlayerController���� ȣ���մϴ�
	// RemoveFromParent(); // ȭ�鿡�� ������ ������ �� ���˴ϴ�. 

protected:
	virtual void NativeConstruct() override; // UMG���� ����ϴ� UI�� �ʱ�ȭ�ϰ� ��ȣ�ۿ��� �����ϴ� ���� �߿��� �Լ��Դϴ�.

	//���� �� UI���� �ǽð����� ���ϴ� �ִϸ��̼� ���� ������Ʈ�ϰ� ���� �� ����մϴ�.
	//bCanEverTick �� ���� ������ �ʿ� ����, NativeTick �� �����ϸ� Unreal Engine�� �̸� �����ϰ� Tick�� ȣ��Ǳ� �����մϴ�.
	//virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY(meta = (BindWidget))
	class UMainMenuButton* StartGameButton;

	UPROPERTY(meta = (BindWidget))
	class UMainMenuButton* QuitGameButton;

	UPROPERTY(meta = (BindWidget))
	class UImage* BackgroundImage;

	UFUNCTION()
	void OnStartGameClicked();

	UFUNCTION()
	void OnQuitGameClicked();
};
