// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

// ��� ��������Ʈ
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBaseCharacterOnDeath);

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
	//Player �� HP �� ���� ���� �����ϴ� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = true))
	class UHealthComponent* PlayerHealthComponent;

	//HP �� UI
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = true))
	class UWidgetComponent* HealthBarComponent;

	UFUNCTION()
	virtual void OnCharacterDeth();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character State")
	bool isDie;

	// ��� ��������Ʈ
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FBaseCharacterOnDeath BaseCharacterOnDeath;
};
