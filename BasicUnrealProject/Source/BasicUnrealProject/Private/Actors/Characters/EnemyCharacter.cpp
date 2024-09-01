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

void AEnemyCharacter::OnCompBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp) //자기 자신과 충돌되었을 경우 배제
	{
		FName BoneName = SweepResult.MyBoneName;
		if (!BoneName.IsEqual("sword_bottom")) return; //검에 해당되는 Bone 에 충돌되었을시에만 공격 처리

		ACharacter* OtherCharacter = Cast<ACharacter>(OtherActor);
		float DamageAmount = 50.0f;
		AController* InstigatorController = GetController(); //AEnemyCharacter (자기자신) 의컨트롤러
		AActor* DamageCauser = this;
		TSubclassOf<UDamageType> DamageType = UDamageType::StaticClass(); //TSubclassOf 는 클래스 타입 자체를 저장한다.

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