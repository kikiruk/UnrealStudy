// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Arrow.generated.h"

UCLASS()
class BASICUNREALPROJECT_API AArrow : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* DefaultComponent;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ArrowMesh;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* CollisionMesh;

	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, Category = "Effects")
	class UParticleSystem* ArrowHitParticleSystem;

public:	
	// Sets default values for this actor's properties
	AArrow();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void BoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// BoxBeginOverlap 함수에 구현되어있는 파티클 임팩트 실행을 한번만 되게끔 하기 위한 bool 값
	bool bHasTriggered;

};
