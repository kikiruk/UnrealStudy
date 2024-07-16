// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/Characters/EnemyCharacter.h"
#include "Controllers/EnemyAIController.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter() : AttackMontage(nullptr)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// AI Controller 클래스 지정
	AIControllerClass = AEnemyAIController::StaticClass();

	RootComponent = GetCapsuleComponent();
	GetMesh()->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<USkeletalMesh> enemySkeletalConstructer(TEXT("/Script/Engine.SkeletalMesh'/Game/MyBlueprint/Enemy_Greystone.Enemy_Greystone'"));

	if (enemySkeletalConstructer.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(enemySkeletalConstructer.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.f), FRotator(0.0f, -90.0f, 0.0f));
	}

	//Enemy 의 animClass를 C++ 코드로 할당
	ConstructorHelpers::FClassFinder<UAnimInstance> animClass(TEXT("/Script/Engine.AnimBlueprint'/Game/MyBlueprint/Animation/Enemy/AnimBP_enemy.AnimBP_enemy_C'"));
	if (animClass.Succeeded())
	{
		GetMesh()->SetAnimClass(animClass.Class);
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage> attackMontageConstructor(TEXT("/Script/Engine.AnimMontage'/Game/MyBlueprint/Animation/Enemy/AM_Attack.AM_Attack'"));

	if (attackMontageConstructor.Succeeded())
	{
		AttackMontage = attackMontageConstructor.Object;
	}
}

void AEnemyCharacter::Attack()
{
	UAnimInstance* AnimIns = GetMesh()->GetAnimInstance();

	if (AnimIns)
	{
		if (AttackMontage && !AnimIns->Montage_IsPlaying(AttackMontage))
		{
			AnimIns->Montage_Play(AttackMontage);
		}
	}
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	USkeletalMeshComponent* MeshComp = GetMesh();
	// Generate Overlap Events를 활성화
	MeshComp->SetGenerateOverlapEvents(true);
	// OnComponentBeginOverlap 이벤트 바인딩
	MeshComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnCompBeginOverlap);
}

void AEnemyCharacter::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Hit@!!!!!!!@!@!@!"));
}

void AEnemyCharacter::OnCompBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlap Began with: %s  !#@#@$@$"), *OtherActor->GetName());
	}
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}