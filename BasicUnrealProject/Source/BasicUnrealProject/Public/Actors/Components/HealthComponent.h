// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


// 체력 변화 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float, NewHealth);

// 사망 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BASICUNREALPROJECT_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
    UHealthComponent();

protected:
    virtual void BeginPlay() override;

    // 체력 변수
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Health" , meta = (AllowPrivateAccess = true))
    float MaxHealth;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Health", meta = (AllowPrivateAccess = true))
    float CurrentHealth;

public: //사용자 정의 함수 및 변수
    // 체력 감소 함수
    void DecreaseHealth(float Amount);

    // 체력 증가 함수
    void IncreaseHealth(float Amount);

    // 체력 변화 델리게이트
    UPROPERTY(BlueprintAssignable, Category="Events")
    FOnHealthChanged OnHealthChanged;

    // 사망 델리게이트
    UPROPERTY(BlueprintAssignable, Category="Events")
    FOnDeath OnDeath;

protected:
    UFUNCTION()
    void Die();
};
