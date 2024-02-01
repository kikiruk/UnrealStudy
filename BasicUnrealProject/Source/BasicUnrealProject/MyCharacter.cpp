// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("MySpringArm"));
	MyCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MyCamera"));

	//Capsule <- CameraSpringArm <- Camera 이렇게 붙임
	MyCameraSpringArm->SetupAttachment(GetCapsuleComponent());
	MyCamera->SetupAttachment(MyCameraSpringArm);

	//카메라 팔 길이 400 으로 설정 하고 회전 (-35.f, 0.f, 0.f)
	MyCameraSpringArm->TargetArmLength = 400.f;
	MyCameraSpringArm->SetRelativeRotation(FRotator(-35.f, 0.f, 0.f));

	ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMesh(
		TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonSparrow/Characters/Heroes/Sparrow/Meshes/Sparrow.Sparrow'"));

	if (SkeletalMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SkeletalMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f));
	}
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	PlayerInputComponent->BindAxis(TEXT("MoveForward"),this, &AMyCharacter::doMoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"),this, &AMyCharacter::doMoveRight);
}

void AMyCharacter::doMoveForward(float val)
{
	UE_LOG(LogTemp, Log, TEXT("Forward : %f"), val);
}

void AMyCharacter::doMoveRight(float val)
{
	UE_LOG(LogTemp, Log, TEXT("Right : %f"), val);
}

