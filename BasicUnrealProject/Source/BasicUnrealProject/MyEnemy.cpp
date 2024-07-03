// Fill out your copyright notice in the Description page of Project Settings.

#include "MyEnemy.h"
#include "MyAIController.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AMyEnemy::AMyEnemy()
{ 
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// AI Controller Ŭ���� ����
	AIControllerClass = AMyAIController::StaticClass();

	RootComponent = GetCapsuleComponent();
	GetMesh()->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<USkeletalMesh> EnemySkeletalconstruct(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonGreystone/Characters/Heroes/Greystone/Meshes/Greystone.Greystone'"));

	if (EnemySkeletalconstruct.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(EnemySkeletalconstruct.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.f), FRotator(0.0f, -90.0f, 0.0f));
	}

	//Enemy �� AnimClass�� C++ �ڵ�� �Ҵ�
	ConstructorHelpers::FClassFinder<UAnimInstance> AnimClass(TEXT("/Script/Engine.AnimBlueprint'/Game/MyBlueprint/Animation/Enemy/AnimBP_enemy.AnimBP_enemy_C'"));
	if (AnimClass.Succeeded())
	{
		GetMesh()->SetAnimClass(AnimClass.Class);
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

