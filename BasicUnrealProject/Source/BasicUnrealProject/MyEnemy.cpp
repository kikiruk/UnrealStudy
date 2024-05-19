// Fill out your copyright notice in the Description page of Project Settings.

#include "MyEnemy.h"
#include "MyAIController.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AMyEnemy::AMyEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// AI Controller 클래스 지정
	AIControllerClass = AMyAIController::StaticClass();

	//RootComponent = GetCapsuleComponent();
	EnemySkeletal = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EnemySkeletal"));
	EnemySkeletal->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<USkeletalMesh> EnemySkeletalconstruct(TEXT("SkeletalMesh'/Game/ParagonGreystone/Characters/Heroes/Greystone/Meshes/Greystone.Greystone'"));
	EnemySkeletal->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -90.f), FRotator(0.f, 0.f, -90.f));

	if (EnemySkeletalconstruct.Succeeded())
	{
		EnemySkeletal->SetSkeletalMesh(EnemySkeletalconstruct.Object);
	}
	
}

// Called when the game starts or when spawned
void AMyEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

