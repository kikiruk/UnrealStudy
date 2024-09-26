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
	virtual void ApplyDamage() override;
protected:
	// 타이머 설정을 위한 TimerHandle을 선언합니다. 타이머가 실행되는 동안 다른 함수에서 타이머를 취소하거나 확인해야 할 수 있으므로 멤버변수로 합니다.
	FTimerHandle TimerHandle_Dissapear;

	int ComboNum; //기본 공격이 몇번째 콤보인지 표시
	bool bSaveAttack;
	class USkeletalMeshComponent* EnemyMesh;
	class UAnimInstance* AnimIns;
	class UEnemyAnimInstance* EnemyAnim;
};