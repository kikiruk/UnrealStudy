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

	/*USceneComponent�� �ð��� ��Ҹ� ���� �������� ������, ���� ���� ������ ��ü�� ��ġ�� �����ϴ� �� �߿��� ������ �մϴ�.
	* USceneComponent�� ���� �ٸ� ������Ʈ���� "�����̳�" ������ �ϸ�, ������ ��ü�� �پ��� �κе��� �ϳ��� ������ �׷�ȭ�� �� �ְ� ���ݴϴ�.
	*/
	DefaultComponent = CreateDefaultSubobject<USceneComponent>(FName("DefaultComponent"));
	SetRootComponent(DefaultComponent);
	ArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh"));

	//static �� �ٿ��־ ���ҽ��� �������°��� ���ʿ��ϰ� �ݺ����� �ʴ´�.
	static ConstructorHelpers::FObjectFinder<UStaticMesh> arrowMeshTemp(
		TEXT("/Script/Engine.StaticMesh'/Game/ParagonSparrow/FX/Meshes/Heroes/Sparrow/Abilities/SM_Sparrow_Arrow.SM_Sparrow_Arrow'"));

	//StaticMesh (ȭ��) ��ġ �� ȸ�� , �浹ä�� �����ϱ�
	if (arrowMeshTemp.Succeeded())
	{
		ArrowMesh->SetStaticMesh(arrowMeshTemp.Object);
		ArrowMesh->SetupAttachment(DefaultComponent);
		ArrowMesh->SetRelativeLocationAndRotation(FVector(100.f, 0.f, 0.f), FRotator(90.f, 0.f, 0.f));
		ArrowMesh->SetCollisionProfileName(FName("NoCollision"));

		//�浹 Box ��ġ �� Scale �� ���� , �θ� ���� 
		CollisionMesh = CreateDefaultSubobject<UBoxComponent>(FName("CollisionMesh"));
		CollisionMesh->SetupAttachment(ArrowMesh);
		CollisionMesh->SetRelativeLocation(FVector(0.f, 0.f, -55.f));
		CollisionMesh->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));
		CollisionMesh->SetGenerateOverlapEvents(true); // Overlap Event Ȱ��ȭ

		/* ProjectileMovementComponent�� UMovementComponent�� ��ӹ޴� ������Ʈ��, Ư�� ����ü�� ��� ó���ϴ� �� ����ȭ�Ǿ� �ֽ��ϴ�.
		* UMovementComponent�� �� �ڽ� Ŭ�������� USceneComponent�� �ڽ��� �ƴϹǷ�,
		* ������Ʈ �������� �θ�� �ڽ� ���� ���踦 �����ϴ� AttachToComponent() �Ǵ� SetupAttachment() �Լ��� ���ԵǾ� ���� �ʽ��ϴ�.
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

	// FTimerHandle�� ��� ������ �����ϰų� �Լ� ������ ���� ������ ����
	FTimerHandle TimerHandle;

	GetWorldTimerManager().SetTimer(
		TimerHandle,            // Ÿ�̸� �ڵ�
		[this]()                // ���� �Լ� - this�� ĸó
		{
			Destroy();          // 3�� �� Destroy() �Լ� ȣ��
		},
		3.0f,                   // Ÿ�̸� ���� �ð� (3��)
		false                   // �ݺ� ���� (false�� ���� ����)
	);
}

void AArrow::BoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bHasTriggered && ArrowHitParticleSystem)
	{
		bHasTriggered = true;
		UE_LOG(LogTemp, Log, TEXT("Particle"));

		// ���� ���忡�� ��ƼŬ �ý����� Ư�� ��ġ�� ����
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),              // ���� ���ؽ�Ʈ
			ArrowHitParticleSystem,          // ����� ��ƼŬ �ý���
			CollisionMesh->GetComponentLocation(),      // ��ƼŬ�� ������ ��ġ
			CollisionMesh->GetComponentRotation(),      // ��ƼŬ�� ȸ��
			true                     // ��ƼŬ�� �ڵ� �ı� ����
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