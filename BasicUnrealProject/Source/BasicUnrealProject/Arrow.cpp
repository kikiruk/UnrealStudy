// Fill out your copyright notice in the Description page of Project Settings.


#include "Arrow.h"
#include "Components/BoxComponent.h"

// Sets default values
AArrow::AArrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh"));
	RootComponent = ArrowMesh;

	//static 을 붙여주어서 리소스를 가져오는것을 불필요하게 반복하지 않는다.
	static ConstructorHelpers::FObjectFinder<UStaticMesh> arrowMeshTemp(
		TEXT("/Script/Engine.StaticMesh'/Game/ParagonSparrow/FX/Meshes/Heroes/Sparrow/Abilities/SM_Sparrow_Arrow.SM_Sparrow_Arrow'"));

	//StaticMesh (화살) 위치 및 회전 , 충돌체널 설정하기
	if (arrowMeshTemp.Succeeded())
	{
		ArrowMesh->SetStaticMesh(arrowMeshTemp.Object);
		ArrowMesh->SetRelativeLocationAndRotation(FVector(100.f, 0.f, 0.f), FRotator(90.f, 0.f, 0.f));
		ArrowMesh->SetCollisionProfileName(FName("NoCollision"));
	}

	//충돌 Box 위치 및 Scale 값 조정 , 부모 설정 
	CollisionMesh = CreateDefaultSubobject<UBoxComponent>(FName("CollisionMesh"));
	CollisionMesh->SetupAttachment(ArrowMesh);
	CollisionMesh->SetRelativeLocation(FVector(0.f, 0.f, -55.f));
	CollisionMesh->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));

}

// Called when the game starts or when spsawned
void AArrow::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

