// Fill out your copyright notice in the Description page of Project Settings.


#include "MyStaticMeshComponent.h"

UMyStaticMeshComponent::UMyStaticMeshComponent()
{
	//Component 에서 Tick 기능 사용 가능한지 Test
	//PrimaryComponentTick.bCanEverTick = true;
	//PrimaryComponentTick.SetTickFunctionEnable(true);
}

void UMyStaticMeshComponent::BeginPlay()
{
	UE_LOG(LogTemp, Log, TEXT("MyStaticMeshComponent attached"));
}




