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

	//Capsule <- CameraSpringArm <- Camera �̷��� ����
	MyCameraSpringArm->SetupAttachment(GetCapsuleComponent());
	MyCamera->SetupAttachment(MyCameraSpringArm);

	//ī�޶� �� ���� 400 ���� ���� �ϰ� ȸ�� (-35.f, 0.f, 0.f)
	MyCameraSpringArm->TargetArmLength = 400.f;
	MyCameraSpringArm->SetRelativeRotation(FRotator(-35.f, 0.f, 0.f));

	//SpringArm �� ȸ���� Controller �� ȸ������ ���� �����̵��� ����
	MyCameraSpringArm->bUsePawnControlRotation = true;

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

	//�𸮾� ȭ�鿡 ���� ���� DebugMessage
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Tick!"));
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"),this, &AMyCharacter::doMoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"),this, &AMyCharacter::doMoveRight);
	PlayerInputComponent->BindAxis(TEXT("MouseUp"),this, &AMyCharacter::doMouseUp);
	PlayerInputComponent->BindAxis(TEXT("MouseRight"),this, &AMyCharacter::doMouseRight);
}

void AMyCharacter::doMoveForward(float val)
{
	AddMovementInput(GetActorForwardVector(), val);
}

void AMyCharacter::doMoveRight(float val)
{
	AddMovementInput(GetActorRightVector(), val);
}

void AMyCharacter::doMouseUp(float val)
{
	AddControllerPitchInput(val);
}

void AMyCharacter::doMouseRight(float val)
{
	AddControllerYawInput(val);
}

