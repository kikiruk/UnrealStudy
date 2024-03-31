// Fill out your copyright notice in the Description page of Project Settings.


#include "Arrow.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

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

	//StaticMesh (ȭ��) ��ġ �� ȸ�� , �浹ü�� �����ϱ�
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

		/* ProjectileMovementComponent�� UMovementComponent�� ��ӹ޴� ������Ʈ��, Ư�� ����ü�� ��� ó���ϴ� �� ����ȭ�Ǿ� �ֽ��ϴ�.
		* UMovementComponent�� �� �ڽ� Ŭ�������� USceneComponent�� �ڽ��� �ƴϹǷ�,
		* ������Ʈ �������� �θ�� �ڽ� ���� ���踦 �����ϴ� AttachToComponent() �Ǵ� SetupAttachment() �Լ��� ���ԵǾ� ���� �ʽ��ϴ�.
		*/
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(FName("ProjectileMovementComponent"));
		ProjectileMovementComponent->SetUpdatedComponent(DefaultComponent);
		ProjectileMovementComponent->InitialSpeed = 3000.f;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
	}
}

// Called when the game starts or when spsawned
void AArrow::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

