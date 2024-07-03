// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "MyBTTask_Attack.generated.h"

/**
 * 
 */
UCLASS()
class BASICUNREALPROJECT_API UMyBTTask_Attack : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UMyBTTask_Attack();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
