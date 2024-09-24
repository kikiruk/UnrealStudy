// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

// 사망 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBaseCharacterOnDeath);
DECLARE_MULTICAST_DELEGATE(FOnStatesChanged);

UCLASS()
class BASICUNREALPROJECT_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
protected:
	//Player 의 HP 와 죽음 등을 관리하는 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = true))
	class UHealthComponent* PlayerHealthComponent;

	//HP 바 UI
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = true))
	class UWidgetComponent* HealthBarComponent;

	UFUNCTION()
	virtual void OnCharacterDeth();
	virtual void OnCharacterStartMoving() { return; };

	virtual void OnMontageNotifyReceived(FName NotifyName) { return; };

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character State")
	bool isDie;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character State")
	bool bIsIdle;

	// 사망 델리게이트
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FBaseCharacterOnDeath BaseCharacterOnDeath;

	//상태 변수 변경 시 브로드캐스트
	FOnStatesChanged OnStatesChanged;
};
