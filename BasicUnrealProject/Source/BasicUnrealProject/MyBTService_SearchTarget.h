// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "MyBTService_SearchTarget.generated.h"

/**
 * 
 */
UCLASS()
class BASICUNREALPROJECT_API UMyBTService_SearchTarget : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UMyBTService_SearchTarget();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetLocationKey;

	UPROPERTY(EditAnywhere, Category = "Detection")
	float SearchRadius = 1000.0f; // Å½»ö ¹Ý°æ (´ÜÀ§: À¯´Ö)
};
