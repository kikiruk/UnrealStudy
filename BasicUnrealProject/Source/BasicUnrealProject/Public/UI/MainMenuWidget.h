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

	// void AddToViewport(int32 ZOrder = 0); // 이 함수는 UI 위젯을 화면에 추가하는 역할을 합니다. 일반적으로 PlayerController에서 호출합니다
	// RemoveFromParent(); // 화면에서 위젯을 제거할 때 사용됩니다. 

protected:
	virtual void NativeConstruct() override; // UMG에서 사용하는 UI를 초기화하고 상호작용을 설정하는 가장 중요한 함수입니다.

	//게임 내 UI에서 실시간으로 변하는 애니메이션 등을 업데이트하고 싶을 때 사용합니다.
	//bCanEverTick 을 따로 설정할 필요 없이, NativeTick 을 구현하면 Unreal Engine이 이를 감지하고 Tick이 호출되기 시작합니다.
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
