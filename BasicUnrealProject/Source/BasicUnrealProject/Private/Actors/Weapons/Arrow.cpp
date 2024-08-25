// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Weapons/Arrow.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

// Sets default values
AArrow::AArrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*USceneComponent는 시각적 요소를 직접 포함하지 않지만, 게임 월드 내에서 객체의 위치를 정의하는 데 중요한 역할을 합니다.
	* USceneComponent는 종종 다른 컴포넌트들의 "컨테이너" 역할을 하며, 복잡한 객체의 다양한 부분들을 하나의 단위로 그룹화할 수 있게 해줍니다.
	*/
	DefaultComponent = CreateDefaultSubobject<USceneComponent>(FName("DefaultComponent"));
	SetRootComponent(DefaultComponent);
	ArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh"));

	//static 을 붙여주어서 리소스를 가져오는것을 불필요하게 반복하지 않는다.
	static ConstructorHelpers::FObjectFinder<UStaticMesh> arrowMeshTemp(
		TEXT("/Script/Engine.StaticMesh'/Game/ParagonSparrow/FX/Meshes/Heroes/Sparrow/Abilities/SM_Sparrow_Arrow.SM_Sparrow_Arrow'"));

	//StaticMesh (화살) 위치 및 회전 , 충돌채널 설정하기
	if (arrowMeshTemp.Succeeded())
	{
		ArrowMesh->SetStaticMesh(arrowMeshTemp.Object);
		ArrowMesh->SetupAttachment(DefaultComponent);
		ArrowMesh->SetRelativeLocationAndRotation(FVector(100.f, 0.f, 0.f), FRotator(90.f, 0.f, 0.f));
		ArrowMesh->SetCollisionProfileName(FName("NoCollision"));

		//충돌 Box 위치 및 Scale 값 조정 , 부모 설정 
		CollisionMesh = CreateDefaultSubobject<UBoxComponent>(FName("CollisionMesh"));
		CollisionMesh->SetupAttachment(ArrowMesh);
		CollisionMesh->SetRelativeLocation(FVector(0.f, 0.f, -55.f));
		CollisionMesh->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));
		CollisionMesh->SetGenerateOverlapEvents(true); // Overlap Event 활성화

		/* ProjectileMovementComponent는 UMovementComponent를 상속받는 컴포넌트로, 특히 투사체의 운동을 처리하는 데 최적화되어 있습니다.
		* UMovementComponent와 그 자식 클래스들은 USceneComponent의 자식이 아니므로,
		* 컴포넌트 계층에서 부모와 자식 간의 관계를 정의하는 AttachToComponent() 또는 SetupAttachment() 함수가 포함되어 있지 않습니다.
		*/
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(FName("ProjectileMovementComponent"));
		ProjectileMovementComponent->SetUpdatedComponent(DefaultComponent);
		ProjectileMovementComponent->InitialSpeed = 3000.f;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> particleMesh(
		TEXT("/Script/Engine.ParticleSystem'/Game/ParagonSparrow/FX/Particles/Sparrow/Abilities/Ultimate/FX/P_Sparrow_UltHit.P_Sparrow_UltHit'"));

	if (particleMesh.Succeeded())
	{
		ArrowHitParticleSystem = particleMesh.Object;
	}

	bHasTriggered = false;
}

// Called when the game starts or when spsawned
void AArrow::BeginPlay()
{
	Super::BeginPlay();
	CollisionMesh->OnComponentBeginOverlap.AddDynamic(this, &AArrow::BoxBeginOverlap);

	// FTimerHandle을 멤버 변수로 선언하거나 함수 내에서 로컬 변수로 선언
	FTimerHandle TimerHandle;

	GetWorldTimerManager().SetTimer(
		TimerHandle,            // 타이머 핸들
		[this]()                // 람다 함수 - this를 캡처
		{
			Destroy();          // 3초 후 Destroy() 함수 호출
		},
		3.0f,                   // 타이머 설정 시간 (3초)
		false                   // 반복 여부 (false로 단일 실행)
	);
}

void AArrow::BoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bHasTriggered && ArrowHitParticleSystem)
	{
		bHasTriggered = true;
		UE_LOG(LogTemp, Log, TEXT("Particle"));

		// 현재 월드에서 파티클 시스템을 특정 위치에 스폰
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),              // 월드 컨텍스트
			ArrowHitParticleSystem,          // 사용할 파티클 시스템
			CollisionMesh->GetComponentLocation(),      // 파티클을 생성할 위치
			CollisionMesh->GetComponentRotation(),      // 파티클의 회전
			true                     // 파티클의 자동 파괴 여부
		);
	}

	ProjectileMovementComponent->StopMovementImmediately();
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
	this->AttachToActor(OtherActor, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));

}

// Called every frame
void AArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}