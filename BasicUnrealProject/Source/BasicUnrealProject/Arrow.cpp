// Fill out your copyright notice in the Description page of Project Settings.


#include "Arrow.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

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

	//StaticMesh (화살) 위치 및 회전 , 충돌체널 설정하기
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

		/* ProjectileMovementComponent는 UMovementComponent를 상속받는 컴포넌트로, 특히 투사체의 운동을 처리하는 데 최적화되어 있습니다.
		* UMovementComponent와 그 자식 클래스들은 USceneComponent의 자식이 아니므로,
		* 컴포넌트 계층에서 부모와 자식 간의 관계를 정의하는 AttachToComponent() 또는 SetupAttachment() 함수가 포함되어 있지 않습니다.
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

