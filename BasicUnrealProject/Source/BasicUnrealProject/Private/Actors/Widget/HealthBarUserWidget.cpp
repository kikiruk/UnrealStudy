// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Widget/HealthBarUserWidget.h"
#include "Components/ProgressBar.h"

UHealthBarUserWidget::UHealthBarUserWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
}

void UHealthBarUserWidget::SetPercent(float persent)
{
	HealthBar->SetPercent(persent);
}
