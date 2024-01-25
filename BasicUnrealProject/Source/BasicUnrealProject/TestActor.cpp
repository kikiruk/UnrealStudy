// Fill out your copyright notice in the Description page of Project Settings.


#include "TestActor.h"
#include "MyStaticMeshComponent.h"

// Sets default values
ATestActor::ATestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("MESH"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh(
		TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Props/SM_Chair.SM_Chair'"));

	if (StaticMesh.Succeeded() == true)
	{
		Mesh->SetStaticMesh(StaticMesh.Object);
	}
}

// Called when the game starts or when spawned
void ATestActor::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Log, TEXT("Hello World"));
	UE_LOG(LogTemp, Warning, TEXT("Hello World"));
	UE_LOG(LogTemp, Error, TEXT("Hello World"));
}

// Called every frame
void ATestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Log, TEXT("DeltaTime : %f"), DeltaTime);
}

