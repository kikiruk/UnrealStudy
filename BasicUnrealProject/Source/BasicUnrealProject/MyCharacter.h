// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

//ĳ���Ͱ� �����Ҷ� broadCast�� ��������Ʈ
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnArrowFiredDelegate);


UCLASS()
class BASICUNREALPROJECT_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:    
	// �̺�Ʈ ��������Ʈ ����
	UPROPERTY(BlueprintAssignable, Category = "Animation")
	FOnArrowFiredDelegate OnArrowFired;

public:
	// Sets default values for this character's properties
	AMyCharacter();
private:
	//����� ���� �Լ�
	void doMoveForward(float val);
	void doMoveRight(float val);
	void doMouseUp(float val);
	void doMouseRight(float val);
	void doChameraArmLengthSetup(float val);
	void doLeftClick();

private:
	/*������ Tick �Լ������� ����� �����Դϴ�.�ٸ��Լ������� ������� ������
	����Yaw �� ����Yaw ���� ���̰� 90 �� �̻��϶� �ε巴�� ����Yaw ������ ȸ���Ϸ��� �ϴµ�,
	ȸ���ϴ� ���� 90�� ���ϰ� �Ǹ� ��������� ����Yaw �� �������� ���ϴ� ������ �߻��Ͽ��� 
	ȸ���� �ؾ��Ѵٴ� bool ���� ����ؾ��ϱ⶧���� ���������� ������.*/
	UPROPERTY(Category = "Animation", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	bool bMustRotateForTick;
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

	//�����������Ʈ Ŭ������ ��ü (�ν��Ͻ�) 
	UPROPERTY(Category = "UI", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UUserWidget* MyPlayerScreenInstance;

};
