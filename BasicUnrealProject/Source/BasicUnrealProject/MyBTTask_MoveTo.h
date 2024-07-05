// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "MyBTTask_MoveTo.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class BASICUNREALPROJECT_API UMyBTTask_MoveTo : public UBTTask_MoveTo
{
	GENERATED_BODY()
	
public:
	UMyBTTask_MoveTo();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
private:
	/** ���� ��ġ�� �˻��� �ݰ� (�������Ʈ���� ���� ����) */
	UPROPERTY(EditAnywhere, Category = "Search")
	float SearchRadius;
};
