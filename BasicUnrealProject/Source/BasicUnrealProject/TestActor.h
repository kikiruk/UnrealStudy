// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestActor.generated.h"

UCLASS()
class BASICUNREALPROJECT_API ATestActor : public AActor
{
	GENERATED_BODY()

	//다른 헤더파일을 보니 멤버변수들은 이렇게 위에 정렬해두는거같아서 그대로 한다. 
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh = nullptr;

public:	
	// Sets default values for this actor's properties
	ATestActor();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
