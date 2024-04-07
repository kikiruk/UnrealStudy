// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnArrowFiredDelegate);

UCLASS()
class BASICUNREALPROJECT_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:    
	// 이벤트 델리게이트 선언
	UPROPERTY(BlueprintAssignable, Category = "Animation")
	FOnArrowFiredDelegate OnArrowFired;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float CameraCharacterDeltaDegree;

public:
	// Sets default values for this character's properties
	AMyCharacter();
private:
	//사용자 정의 함수
	void doMoveForward(float val);
	void doMoveRight(float val);
	void doMouseUp(float val);
	void doMouseRight(float val);
	void doChameraArmLengthSetup(float val);
	void doLeftClick();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

private:
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* MyCamera;

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* MyCameraSpringArm;

	//위젯블루프린트 클래스의 객체 (인스턴스) 
	UPROPERTY(Category = "UI", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UUserWidget* MyPlayerScreenInstance;

};
