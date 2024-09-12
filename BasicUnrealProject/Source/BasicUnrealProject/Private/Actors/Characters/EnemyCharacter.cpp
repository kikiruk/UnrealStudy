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

	// Overlap ����� ������ �迭
	TArray<FOverlapResult> overlapResults;
	FVector capsuleLocation = GetActorLocation() + GetActorForwardVector() * 60;// ĸ���� ��ġ
	float capsuleRadius = 50.f;                    // ĸ���� ������
	float capsuleHalfHeight = 120.f;                // ĸ���� ���� (���ݰ�)
	FRotator capsuleRotator = GetActorForwardVector().Rotation();     // ĸ���� ȸ��
	capsuleRotator += FRotator(90.0f, 0.0f, 0.0f);
	FQuat capsuleRotation = capsuleRotator.Quaternion();
	
	// �浹 ������ ĸ���� ����
	FCollisionShape CollisionShape = FCollisionShape::MakeCapsule(capsuleRadius, capsuleHalfHeight);

	// OverlapMultiByChannel �Լ� ȣ��
	bool bHasOverlapped = GetWorld()->OverlapMultiByChannel(
		overlapResults,               // Overlap ����� ����� �迭
		capsuleLocation,              // �������� �߽� ��ġ
		capsuleRotation,              // �������� ȸ��
		ECollisionChannel::ECC_Pawn,  // �浹 ä�� ����
		CollisionShape                // �浹 ���� (��ü)
	);

	// ����� ĸ�� �׸���
	UKismetSystemLibrary::DrawDebugCapsule(GetWorld(), capsuleLocation, capsuleHalfHeight, capsuleRadius, capsuleRotator, FLinearColor::Green, 5.0f, 2.0f);

	if (bHasOverlapped) // �浹�� ����Ǿ��ٸ�
	{
		for (const FOverlapResult& result : overlapResults)
		{
			AActor* resultActor = result.GetActor();
			if (!resultActor || resultActor == this) continue; //resultActor �� ��ȿ���� �ʰų�, �ڱ� �ڽ��� ��� ����

			ABaseCharacter* resultBaseCharacter = Cast<ABaseCharacter>(resultActor);
			AController* thisController = GetController();

			// resultBaseCharacter�� thisController�� ��� ��ȿ���� Ȯ�� �� ApplyDamage ȣ��
			if (resultBaseCharacter && thisController)
			{
				UGameplayStatics::ApplyDamage(resultBaseCharacter, 50.0f, thisController, this, UDamageType::StaticClass());
			}
		}
	}
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
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