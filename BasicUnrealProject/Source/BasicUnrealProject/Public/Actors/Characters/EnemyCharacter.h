// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "EnemyCharacter.generated.h"


UCLASS()
class BASICUNREALPROJECT_API AEnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void Attack();

public:
	virtual void OnMontageNotifyReceived(FName NotifyName) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UFUNCTION()
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	virtual void OnCharacterDeth() override;
	virtual void OnCharacterStartMoving() override;
protected:
	// Ÿ�̸� ������ ���� TimerHandle�� �����մϴ�. Ÿ�̸Ӱ� ����Ǵ� ���� �ٸ� �Լ����� Ÿ�̸Ӹ� ����ϰų� Ȯ���ؾ� �� �� �����Ƿ� ��������� �մϴ�.
	FTimerHandle TimerHandle_Dissapear;

	int ComboNum; //�⺻ ������ ���° �޺����� ǥ��
	bool bSaveAttack;
	class USkeletalMeshComponent* EnemyMesh;
	class UAnimInstance* AnimIns;
	class UEnemyAnimInstance* EnemyAnim;
};