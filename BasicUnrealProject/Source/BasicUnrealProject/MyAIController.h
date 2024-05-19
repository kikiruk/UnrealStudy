// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h" // �� ����� ���Ե��� �ʾƵ� �۵��ǳ�, �������� ������������ ���ؼ� ��������� �����ϴ�.
#include "BehaviorTree/BlackboardComponent.h"
#include "MyAIController.generated.h"

/**
 * 
 */
UCLASS()
class BASICUNREALPROJECT_API AMyAIController : public AAIController
{
	GENERATED_BODY()
public:
	AMyAIController();


public:
	virtual void BeginPlay() override;

	virtual void Tick(float deltaSecond) override;

private:
	UPROPERTY(EditAnywhere, Category = "AI")
	UBehaviorTree* BehaviorTreeAsset;

	UPROPERTY(Transient)
	UBlackboardComponent* BlackboardComponent;
};
