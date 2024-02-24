// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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

	//[ȸ�� ����]
	bUseControllerRotationYaw = false; // ĳ���Ͱ� ��Ʈ�ѷ� ȸ���� ���� �ڵ����� ȸ������ �ʰ� ����
	MyCameraSpringArm->bUsePawnControlRotation = true; //SpringArm �� ȸ���� Controller �� ȸ������ ���� �����̵��� ����
	MyCamera->bUsePawnControlRotation = false;// ī�޶� ��ü������ ȸ���ϵ��� ���� (ĳ������ ȸ���� ������ ����)
	

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

	GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APlayerController* MyController = Cast<APlayerController>(Controller);
	if (!MyController) {
		return;
	}

	// ���� ������ Yaw ȸ���� ��Ʈ�ѷ��� Yaw ȸ���� �����ɴϴ�.
	// FMath::UnwindDegrees �� -180������ 180 ���� ������ ����ȭ ���Ѽ� 0������ 360���� ǥ��Ǵ°���Ǽ��� ���۴ϴ�.
	float CurrentYaw = FMath::UnwindDegrees(GetActorRotation().Yaw);
	float TargetYaw = FMath::UnwindDegrees(MyController->GetControlRotation().Yaw);

	// �������� ���� ���ٽ� : ���� Ű �� �ϳ��� �����ִ��� Ȯ��
	auto IsAnyMovementKeyPressed = [MyController]() -> bool {
		return MyController->IsInputKeyDown(EKeys::W) ||
			MyController->IsInputKeyDown(EKeys::A) ||
			MyController->IsInputKeyDown(EKeys::S) ||
			MyController->IsInputKeyDown(EKeys::D);
		};

	// ������ ȸ���� ����Ǿ���, �̵� Ű�� �����ִ� ��쿡�� ȸ�� ������Ʈ
	if (CurrentYaw != TargetYaw && IsAnyMovementKeyPressed())
	{
		// �� Yaw �� ������ ���̸� ����մϴ�. FindDeltaAngleDegrees ���� ��������ν� ĳ���� ȸ���� �ݴ�� ȸ���ϴ� ������ �����մϴ�.
		float YawDifference = FMath::FindDeltaAngleDegrees(CurrentYaw, TargetYaw);

		FRotator NewRotation = GetActorRotation();
		float NewRotationYaw = FMath::FInterpTo(CurrentYaw, CurrentYaw + YawDifference, DeltaTime, 10.0f); //�ε巴�� ȸ���մϴ�.
		NewRotation.Yaw = NewRotationYaw;
		SetActorRotation(NewRotation);
	}


	FVector ActorVelocity = GetMovementComponent()->Velocity; //���� ��ǥ�迡�� character �� Velocity
	FVector ActorLocalVelocity = GetActorRotation().UnrotateVector(ActorVelocity); //�� ���� Camera ���� character �� Velocity

	GetCharacterMovement()->MaxWalkSpeed = 600.f;

	if (ActorLocalVelocity.X > 0 && MyController->IsInputKeyDown(EKeys::LeftShift)) //ĳ���Ͱ� �������� �����̰�, Shift�� ������ �ִٸ�
	{
		GetCharacterMovement()->MaxWalkSpeed = 1200.f; //ĳ���� �ִ�ӵ� ����
	}

	UE_LOG(LogTemp, Log, TEXT("%f"), ActorLocalVelocity.Length());
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"),this, &AMyCharacter::doMoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"),this, &AMyCharacter::doMoveRight);
	PlayerInputComponent->BindAxis(TEXT("MouseUp"),this, &AMyCharacter::doMouseUp);
	PlayerInputComponent->BindAxis(TEXT("MouseRight"),this, &AMyCharacter::doMouseRight);
	PlayerInputComponent->BindAxis(TEXT("MouseWheel"), this, &AMyCharacter::doChameraArmLengthSetup);

	//�𸮾� �⺻ ���� �Լ� Jump �� �״�� �����.
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AMyCharacter::Jump);
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

void AMyCharacter::doChameraArmLengthSetup(float val)
{
	//ī�޶� ���� ���� ����
	float NewCameraArmLength = MyCameraSpringArm->TargetArmLength + val * 5;
	NewCameraArmLength = FMath::Min(NewCameraArmLength, 800.f);
	NewCameraArmLength = FMath::Max(NewCameraArmLength, 200.f);
	MyCameraSpringArm->TargetArmLength = NewCameraArmLength;
}
