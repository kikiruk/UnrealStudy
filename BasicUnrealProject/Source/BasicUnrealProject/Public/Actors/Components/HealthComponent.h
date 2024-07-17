// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


// ü�� ��ȭ ��������Ʈ
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float, NewHealth);

// ��� ��������Ʈ
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

    // ü�� ����
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Health" , meta = (AllowPrivateAccess = true))
    float MaxHealth;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Health", meta = (AllowPrivateAccess = true))
    float CurrentHealth;

public: //����� ���� �Լ� �� ����
    // ü�� ���� �Լ�
    void DecreaseHealth(float Amount);

    // ü�� ���� �Լ�
    void IncreaseHealth(float Amount);

    // ü�� ��ȭ ��������Ʈ
    UPROPERTY(BlueprintAssignable, Category="Events")
    FOnHealthChanged OnHealthChanged;

    // ��� ��������Ʈ
    UPROPERTY(BlueprintAssignable, Category="Events")
    FOnDeath OnDeath;

protected:
    UFUNCTION()
    void Die();
};
