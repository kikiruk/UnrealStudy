// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/Characters/PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/CharacterMovementComponent.h" // �̰� �� �־���ϴ��� Ȯ��
#include "Actors/Components/HealthComponent.h"
#include "Components/WidgetComponent.h"
#include "Actors/Widget/HealthBarUserWidget.h"

// Sets default values
APlayerCharacter::APlayerCharacter() :
	MyCamera(nullptr), MyCameraSpringArm(nullptr), MyPlayerScreenInstance(nullptr), bMustRotateForTick(false),
	bCanBeFireArrow(true)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("MySpringArm"));
	MyCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MyCamera"));
	PlayerHealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthBarComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarComponent"));

	//2D ��ũ���� HP ���� ����
	HealthBarComponent->SetWidgetSpace(EWidgetSpace::Screen);

	//Capsule <- CameraSpringArm <- Camera �̷��� ����
	MyCameraSpringArm->SetupAttachment(GetCapsuleComponent());
	MyCamera->SetupAttachment(MyCameraSpringArm);
	HealthBarComponent->SetupAttachment(GetCapsuleComponent());

	//ī�޶� �� ���� 400 ���� ���� �ϰ� ȸ�� (-35.f, 0.f, 0.f)
	MyCameraSpringArm->TargetArmLength = 400.f;

	//SocketOffset �Ӽ��� �̿��Ͽ� ������ ������Ʈ�� �⺻ ��ġ�� �̼� �����Ѵ�
	//Aim�� ȭ�鿡 ������ ���� ĳ���� ������ ��� ���� �ø��������̴�. �п����������� ���������� ������ ���� ����� Ȯ���� �ƴϴ�.
	MyCameraSpringArm->SocketOffset = FVector(0.f, 120.f, 75.f);
	MyCameraSpringArm->SetRelativeRotation(FRotator(-35.f, 0.f, 0.f));
	HealthBarComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 116.0f));


	//[ȸ�� ����]
	bUseControllerRotationYaw = false; // ĳ���Ͱ� ��Ʈ�ѷ� ȸ���� ���� �ڵ����� ȸ������ �ʰ� ����
	MyCameraSpringArm->bUsePawnControlRotation = true; //SpringArm �� ȸ���� Controller �� ȸ������ ���� �����̵��� ����
	MyCamera->bUsePawnControlRotation = false; // ī�޶� ��ü������ ȸ���ϵ��� ���� (ĳ������ ȸ���� ������ ����)

	ConstructorHelpers::FClassFinder<UHealthBarUserWidget> healthBarConstructer(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/MyBlueprint/WBP_HealthBarUserWidget.WBP_HealthBarUserWidget_C'"));

	if (healthBarConstructer.Succeeded())
	{
		HealthBarComponent->SetWidgetClass(healthBarConstructer.Class);
	}

	ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMesh(
		TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonSparrow/Characters/Heroes/Sparrow/Meshes/Sparrow.Sparrow'"));

	if (SkeletalMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SkeletalMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f));
	}

	//�������Ʈ���� �Ҵ����ִ� Chracter �� AnimClass�� C++ �ڵ�� �Ҵ�
	ConstructorHelpers::FClassFinder<UAnimInstance> AnimClass(TEXT("/Script/Engine.AnimBlueprint'/Game/MyBlueprint/Animation/Player/AnimBP_Player.AnimBP_Player_C'"));
	if (AnimClass.Succeeded())
	{
		GetMesh()->SetAnimClass(AnimClass.Class);
	}
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	TSubclassOf<UUserWidget> MyPlayerScreenClass = LoadClass<UUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/MyBlueprint/WBP_PlayerScreen.WBP_PlayerScreen_C'"));

	//MyPlayerScreen �� TSubclassOf<UUserWidget> �̸�, �����������Ʈ Ŭ�����̰� �̰� ��üȭ�Ͽ� ȭ�鿡 ���̴� �����̴�
	if (IsValid(MyPlayerScreenClass))
	{
		/*MyChracter �� GetController �� PlayerController ����Ȯ�����ָ鼭
		���� PlayerController �Ӹ��ƴ϶� �ٸ��뵵�� ���뼺�� ����ϸ鼭,
		 �������� �ο���
		*/
		APlayerController* playerController = Cast<APlayerController>(GetController());
		if (IsValid(playerController))
		{
			MyPlayerScreenInstance = CreateWidget<UUserWidget>(playerController, MyPlayerScreenClass);
		}

		if (IsValid(MyPlayerScreenInstance))
		{
			//MyPlayerScreenInstance->AddToViewport(); // ������� Aim ��õ��� �Ⱥ��̰� ����
		}
	}

	GetCharacterMovement()->MaxWalkSpeed = 600.f;

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnCapsuleCompBeginOverlap);
}

void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void APlayerCharacter::ResetCanBeFireArrowTrue()
{
	bCanBeFireArrow = true;
}

void APlayerCharacter::OnCapsuleCompBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("OnCapsuleCompBeginOverlap@@@@@@@@@@@@@@@@@@"))
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
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

	//���� ȸ�� Rotation �ʱ�ȭ
	FRotator PurposeRotation = GetActorRotation();
	float PurposeRotationYaw = GetActorRotation().Yaw;

	// ������ ȸ���� ����� ��쿡 ȸ�� ������Ʈ 
	if (!FMath::IsNearlyEqual(CurrentYaw, TargetYaw, 5.f)) // 5�� ������ ���̴� ���� ������ ������
	{
		//FindDeltaAngleDegrees �Լ��� ��������ν� ĳ���� ȸ���� �ݴ�� ȸ���ϴ� ������ �����մϴ�.
		float CameraCharacterDeltaDegree = FMath::FindDeltaAngleDegrees(CurrentYaw, TargetYaw);
		bool isChracterFalling = GetCharacterMovement()->IsFalling();

		if (!isChracterFalling) //���߿� �� �ִ� ���� �ƴҶ� 
		{
			if (IsAnyMovementKeyPressed()) //�̵� Ű�� �����ִ� ��� ĳ���� ��ü ȸ�� (�ƴҽ� ��ü�� ȸ��)
			{
				PurposeRotationYaw = FMath::FInterpTo(CurrentYaw, CurrentYaw + CameraCharacterDeltaDegree, DeltaTime, 10.0f); //�ε巴�� ȸ���մϴ�.
			}
			// ���̰� 130�� �̻��ϰ�� �� ���̰� ������������ ȸ���� �� ȸ���ؾ��ϴ������θ� bMustRotateForTick(��������) ��������
			else if (FMath::Abs(CameraCharacterDeltaDegree) >= 130.f)
			{
				bMustRotateForTick = true;
			}

			if (bMustRotateForTick == true)
			{
				PurposeRotationYaw = FMath::FInterpTo(CurrentYaw, CurrentYaw + CameraCharacterDeltaDegree, DeltaTime, 3.0f); //�ε巴�� ȸ���մϴ�.
			}
		}

		PurposeRotation.Yaw = PurposeRotationYaw;
		SetActorRotation(PurposeRotation);
	}
	else if (bMustRotateForTick == true)
	{
		//�ִ� 5�� ���� ���� ���� �����Ƿ� Yaw ���� Target Yaw �� ������
		bMustRotateForTick = false;
		PurposeRotation.Yaw = TargetYaw;
		SetActorRotation(PurposeRotation);
	}

	FVector ActorVelocity = GetCharacterMovement()->Velocity; //���� ��ǥ�迡�� character �� Velocity
	FVector ActorLocalVelocity = GetActorRotation().UnrotateVector(ActorVelocity); //�� ���� Camera ���� character �� Velocity

	GetCharacterMovement()->MaxWalkSpeed = 600.f;

	if (ActorLocalVelocity.X > 0 && MyController->IsInputKeyDown(EKeys::LeftShift)) //ĳ���Ͱ� �������� �����̰�, Shift�� ������ �ִٸ�
	{
		GetCharacterMovement()->MaxWalkSpeed = 1200.f; //ĳ���� �ִ�ӵ� ����
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APlayerCharacter::doMoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &APlayerCharacter::doMoveRight);
	PlayerInputComponent->BindAxis(TEXT("MouseUp"), this, &APlayerCharacter::doMouseUp);
	PlayerInputComponent->BindAxis(TEXT("MouseRight"), this, &APlayerCharacter::doMouseRight);
	PlayerInputComponent->BindAxis(TEXT("MouseWheel"), this, &APlayerCharacter::doChameraArmLengthSetup);

	//�𸮾� �⺻ ���� �Լ� Jump �� �״�� �����.
	PlayerInputComponent->BindAction(TEXT("LeftClick"), EInputEvent::IE_Pressed, this, &APlayerCharacter::doLeftClick);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Jump);
}

float APlayerCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	PlayerHealthComponent->DecreaseHealth(ActualDamage);
	UHealthBarUserWidget* PlayerHealthBar = Cast<UHealthBarUserWidget>(HealthBarComponent->GetWidget());

	if (PlayerHealthBar)
	{
		PlayerHealthBar->SetPercent(PlayerHealthComponent->GetPercent());
	}

	return ActualDamage;
}

void APlayerCharacter::doMoveForward(float val)
{
	AddMovementInput(GetActorForwardVector(), val);
}

void APlayerCharacter::doMoveRight(float val)
{
	AddMovementInput(GetActorRightVector(), val);
}

void APlayerCharacter::doMouseUp(float val)
{
	AddControllerPitchInput(val);
}

void APlayerCharacter::doMouseRight(float val)
{
	AddControllerYawInput(val);
}

void APlayerCharacter::doChameraArmLengthSetup(float val)
{
	//ī�޶� ���� ���� ����
	float NewCameraArmLength = MyCameraSpringArm->TargetArmLength + val * 5;
	NewCameraArmLength = FMath::Min(NewCameraArmLength, 800.f);
	NewCameraArmLength = FMath::Max(NewCameraArmLength, 200.f);
	MyCameraSpringArm->TargetArmLength = NewCameraArmLength;
}

void APlayerCharacter::doLeftClick()
{
	if (bCanBeFireArrow)
	{
		bCanBeFireArrow = false;

		//���̷�Ż �޽��� arrow_anchor ��� �̸��� ���� ������ �����´�.
		FTransform SocketTranceform = GetMesh()->GetSocketTransform(FName("arrow_anchor"));
		FVector SocketLocation = SocketTranceform.GetLocation();
		FRotator SocketRotation = SocketTranceform.GetRotation().Rotator();

		/*FActorSpawnParameters ����ü�� Unreal Engine���� ���͸� ����(����)�� �� ���Ǵ� �Ű����� ������ ����
		* Owner �� �����������ν� ȭ���� ������ ���� �߻�Ǿ������� �� �� �ֽ��ϴ�.
		* �̸� ���� ȭ�쿡 ���� ���ظ� ����ϰų� ���� ������ ������ �� �����ڸ� ������� Ư���� ��Ģ�� ������ �� �ֽ��ϴ�.
		*/
		FActorSpawnParameters params;
		params.Owner = this;

		GetWorld()->SpawnActor<AActor>(ArrowBlueprint, SocketLocation, SocketRotation, params);

		// �̺�Ʈ ��������Ʈ ȣ��
		OnArrowFired.Broadcast();
	}
}
