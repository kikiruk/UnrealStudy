// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/Characters/EnemyCharacter.h"
#include "Controllers/EnemyAIController.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter() : AttackMontage(nullptr)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// AI Controller Ŭ���� ����
	AIControllerClass = AEnemyAIController::StaticClass();

	RootComponent = GetCapsuleComponent();
	GetMesh()->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<USkeletalMesh> enemySkeletalConstructer(TEXT("/Script/Engine.SkeletalMesh'/Game/MyBlueprint/Enemy_Greystone.Enemy_Greystone'"));

	if (enemySkeletalConstructer.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(enemySkeletalConstructer.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.f), FRotator(0.0f, -90.0f, 0.0f));
	}

	//Enemy �� animClass�� C++ �ڵ�� �Ҵ�
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
	// Generate Overlap Events�� Ȱ��ȭ
	MeshComp->SetGenerateOverlapEvents(true);
	// OnComponentBeginOverlap �̺�Ʈ ���ε�
	MeshComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnCompBeginOverlap);
}

void AEnemyCharacter::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	return;
}

void AEnemyCharacter::OnCharacterDeth()
{
	Super::OnCharacterDeth();

	GetCharacterMovement()->DisableMovement();
	SetActorEnableCollision(false); // �ݸ��� ��Ȱ��ȭ

	GetWorldTimerManager().SetTimer( // Ÿ�̸� ����, 10�� �� ����
		TimerHandle_Dissapear,		// Ÿ�̸� ������ ���� �ɹ����� FTimerHandle
		FTimerDelegate::CreateLambda([this]() { // ���ٽ�
			GetMesh()->SetVisibility(false);	// ������ �ʰ� ����
			SetActorHiddenInGame(true);			// 
			SetLifeSpan(1.0f); // 1�� �Ŀ� ������ ����
		}),
		10.0f,
		false
	);
}

void AEnemyCharacter::OnCompBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp) //�ڱ� �ڽŰ� �浹�Ǿ��� ��� ����
	{
		FName BoneName = SweepResult.MyBoneName;
		if (!BoneName.IsEqual("sword_bottom")) return; //�˿� �ش�Ǵ� Bone �� �浹�Ǿ����ÿ��� ���� ó��

		ACharacter* OtherCharacter = Cast<ACharacter>(OtherActor);
		float DamageAmount = 50.0f;
		AController* InstigatorController = GetController(); //AEnemyCharacter (�ڱ��ڽ�) ����Ʈ�ѷ�
		AActor* DamageCauser = this;
		TSubclassOf<UDamageType> DamageType = UDamageType::StaticClass(); //TSubclassOf �� Ŭ���� Ÿ�� ��ü�� �����Ѵ�.

		if (OtherCharacter && InstigatorController)
		{
			UGameplayStatics::ApplyDamage(OtherActor, DamageAmount, InstigatorController, DamageCauser, DamageType);
		}
		else if (!OtherCharacter && !InstigatorController)
		{
			UE_LOG(LogTemp, Error, TEXT("AEnemyCharacter::OnCompBeginOverlap, OtherCharacter or InstigatorController is NULL"));
		}
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