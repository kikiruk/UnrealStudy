// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "PlayerCharacter.generated.h"

//ĳ���Ͱ� �����Ҷ� broadCast�� ��������Ʈ
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnArrowFiredDelegate);


UCLASS()
class BASICUNREALPROJECT_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// �̺�Ʈ ��������Ʈ ����
	UPROPERTY(BlueprintAssignable, Category = "Animation")
	FOnArrowFiredDelegate OnArrowFired;

public:
	// Sets default values for this character's properties
	APlayerCharacter();
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
	����Yaw �� ����Yaw ���� ���̰� 130 �� �̻��϶� �ε巴�� ����Yaw ������ ȸ���Ϸ��� �ϴµ�,
	ȸ���ϴ� ���� 130�� ���ϰ� �Ǹ� ��������� ����Yaw �� �������� ���ϴ� ������ �߻��Ͽ���
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

private:
	// �� ������ 
	UPROPERTY(Category = "Attack", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	bool bCanBeFireArrow;

public:
	void ResetCanBeFireArrowTrue();
	
protected:
	UFUNCTION()
	void OnCapsuleCompBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
