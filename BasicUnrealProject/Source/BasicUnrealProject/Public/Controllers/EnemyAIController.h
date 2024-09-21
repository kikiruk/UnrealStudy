// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeTypes.h" // FBlackboardKeySelector �� ���� ����� ������ ���� ���� ��� ������.
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class BASICUNREALPROJECT_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
public:
	AEnemyAIController();

public:
	virtual void BeginPlay() override;

	virtual void Tick(float deltaSecond) override;

public:
	void SetMontageEndedKeyTrue();

private:
	UPROPERTY(EditAnywhere, Category = "AI")
	class UBehaviorTree* BehaviorTreeAsset;

	UPROPERTY(Transient)
	class UBlackboardComponent* BlackboardComponent;

private:
	UPROPERTY(EditAnywhere, Category = "BlackboardKey")
	FBlackboardKeySelector IsDieKey;

	UPROPERTY(EditAnywhere, Category = "BlackboardKey")
	FBlackboardKeySelector IsLevelStartMontageEnded;

	UFUNCTION()
	void OwnerCharacerDeath();
};
