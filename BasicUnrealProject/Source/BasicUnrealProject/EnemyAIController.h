// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h" // 이 헤더는 포함되지 않아도 작동되나, 가독성과 유지보수성을 위해서 명시적으로 적습니다.
#include "BehaviorTree/BlackboardComponent.h"
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

private:
	UPROPERTY(EditAnywhere, Category = "AI")
	UBehaviorTree* BehaviorTreeAsset;

	UPROPERTY(Transient)
	UBlackboardComponent* BlackboardComponent;
};
