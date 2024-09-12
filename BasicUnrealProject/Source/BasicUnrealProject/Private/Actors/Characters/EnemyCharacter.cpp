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

	// Overlap 결과를 저장할 배열
	TArray<FOverlapResult> overlapResults;
	FVector capsuleLocation = GetActorLocation() + GetActorForwardVector() * 60;// 캡슐의 위치
	float capsuleRadius = 50.f;                    // 캡슐의 반지름
	float capsuleHalfHeight = 120.f;                // 캡슐의 높이 (절반값)
	FRotator capsuleRotator = GetActorForwardVector().Rotation();     // 캡슐의 회전
	capsuleRotator += FRotator(90.0f, 0.0f, 0.0f);
	FQuat capsuleRotation = capsuleRotator.Quaternion();
	
	// 충돌 영역을 캡슐로 설정
	FCollisionShape CollisionShape = FCollisionShape::MakeCapsule(capsuleRadius, capsuleHalfHeight);

	// OverlapMultiByChannel 함수 호출
	bool bHasOverlapped = GetWorld()->OverlapMultiByChannel(
		overlapResults,               // Overlap 결과가 저장될 배열
		capsuleLocation,              // 오버랩의 중심 위치
		capsuleRotation,              // 오버랩의 회전
		ECollisionChannel::ECC_Pawn,  // 충돌 채널 설정
		CollisionShape                // 충돌 영역 (구체)
	);

	// 디버그 캡슐 그리기
	UKismetSystemLibrary::DrawDebugCapsule(GetWorld(), capsuleLocation, capsuleHalfHeight, capsuleRadius, capsuleRotator, FLinearColor::Green, 5.0f, 2.0f);

	if (bHasOverlapped) // 충돌이 검출되었다면
	{
		for (const FOverlapResult& result : overlapResults)
		{
			AActor* resultActor = result.GetActor();
			if (!resultActor || resultActor == this) continue; //resultActor 가 유효하지 않거나, 자기 자신일 경우 생략

			ABaseCharacter* resultBaseCharacter = Cast<ABaseCharacter>(resultActor);
			AController* thisController = GetController();

			// resultBaseCharacter와 thisController가 모두 유효한지 확인 후 ApplyDamage 호출
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
	SetActorEnableCollision(false); // 콜리전 비활성화

	GetWorldTimerManager().SetTimer( // 타이머 설정, 10초 뒤 실행
		TimerHandle_Dissapear,		// 타이머 설정을 위한 맴버변수 FTimerHandle
		FTimerDelegate::CreateLambda([this]() { // 람다식
			GetMesh()->SetVisibility(false);	// 보이지 않게 설정
			SetActorHiddenInGame(true);			// 
			SetLifeSpan(1.0f); // 1초 후에 완전히 제거
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