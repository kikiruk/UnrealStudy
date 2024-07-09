// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_EnemyMain.generated.h"

/**
 * 
 */
UCLASS()
class BASICUNREALPROJECT_API UBTService_EnemyMain : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTService_EnemyMain();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetLocationKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetDistanceKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetKey;

	UPROPERTY(EditAnywhere, Category = "Detection")
	float SearchRadius = 1000.0f; // Å½»ö ¹Ý°æ (´ÜÀ§: À¯´Ö)

	UPROPERTY(EditAnywhere, Category = "Detection")
	float searchPawnDistance = 0.f; // Å½»ö ¹Ý°æ (´ÜÀ§: À¯´Ö)
};
