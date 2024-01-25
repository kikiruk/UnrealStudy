// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "MyStaticMeshComponent.generated.h"

/**
 * 
 */
UCLASS()
class BASICUNREALPROJECT_API UMyStaticMeshComponent : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	UMyStaticMeshComponent();

	void BeginPlay();
};

