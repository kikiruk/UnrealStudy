// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestActor.generated.h"

UCLASS()
class BASICUNREALPROJECT_API ATestActor : public AActor
{
	GENERATED_BODY()

	//�ٸ� ��������� ���� ����������� �̷��� ���� �����صδ°Ű��Ƽ� �״�� �Ѵ�. 
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
