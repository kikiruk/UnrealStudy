// Fill out your copyright notice in the Description page of Project Settings.


#include "MyStaticMeshComponent.h"

UMyStaticMeshComponent::UMyStaticMeshComponent()
{
	//Component ���� Tick ��� ��� �������� Test
	//PrimaryComponentTick.bCanEverTick = true;
	//PrimaryComponentTick.SetTickFunctionEnable(true);
}

void UMyStaticMeshComponent::BeginPlay()
{
	UE_LOG(LogTemp, Log, TEXT("MyStaticMeshComponent attached"));
}




