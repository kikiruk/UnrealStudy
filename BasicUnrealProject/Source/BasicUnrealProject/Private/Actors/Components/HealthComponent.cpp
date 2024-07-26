// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Components/HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent() : MaxHealth(10000.f), CurrentHealth(MaxHealth)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// ...
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

void UHealthComponent::DecreaseHealth(float Amount)
{
	CurrentHealth -= Amount;
	if (CurrentHealth < 0.0f)
	{
		CurrentHealth = 0.0f;
	}

	OnHealthChanged.Broadcast(CurrentHealth);

	UE_LOG(LogTemp, Log, TEXT("HP : %f"), CurrentHealth); //디버그용 제거가능 

	if (CurrentHealth <= 0.0f)
	{
		Die();
	}
}

void UHealthComponent::IncreaseHealth(float Amount)
{
	CurrentHealth += Amount;
	if (CurrentHealth > MaxHealth)
	{
		CurrentHealth = MaxHealth;
	}

	OnHealthChanged.Broadcast(CurrentHealth);
}

float UHealthComponent::GetPercent()
{
	return CurrentHealth / MaxHealth;
}

void UHealthComponent::Die()
{
	OnDeath.Broadcast();
	// 추가 사망 처리 로직
	UE_LOG(LogTemp, Warning, TEXT("Character Died"));
}