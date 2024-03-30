// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AMyCharacter::AMyCharacter() :
	MyCamera(nullptr), MyCameraSpringArm(nullptr), MyFireMontage(nullptr)
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

	//SocketOffset 속성을 이용하여 부착된 컴포넌트의 기본 위치를 미세 조정한다
	//Aim이 화면에 나오는 것을 캐릭터 오른쪽 어깨 위로 올리기위함이다. 학원수업내용을 따라기기위해 했지만 좋은 방법은 확실히 아니다.
	MyCameraSpringArm->SocketOffset = FVector(0.f, 120.f, 75.f); 
	MyCameraSpringArm->SetRelativeRotation(FRotator(-35.f, 0.f, 0.f));

	//[회전 설정]
	bUseControllerRotationYaw = false; // 캐릭터가 컨트롤러 회전을 따라 자동으로 회전하지 않게 설정
	MyCameraSpringArm->bUsePawnControlRotation = true; //SpringArm 의 회전을 Controller 의 회전값에 따라 움직이도록 설정
	MyCamera->bUsePawnControlRotation = false; // 카메라가 자체적으로 회전하도록 설정 (캐릭터의 회전을 따르지 않음)
	

	ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMesh(
		TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonSparrow/Characters/Heroes/Sparrow/Meshes/Sparrow.Sparrow'"));

	if (SkeletalMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SkeletalMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f));
	}

	//Fire 몽타주 초기화 코드
	ConstructorHelpers::FObjectFinder<UAnimMontage> FireAnimMontageConstructer(TEXT(
		"'/Game/ParagonSparrow/Characters/Heroes/Sparrow/Animations/Primary_Fire_Med_Montage.Primary_Fire_Med_Montage'"));
	
	if (FireAnimMontageConstructer.Succeeded())
	{
		MyFireMontage = FireAnimMontageConstructer.Object;
	}

	//블루프린트에서 할당해주던 Chracter 의 AnimClass를 C++ 코드로 할당
	ConstructorHelpers::FClassFinder<UAnimInstance> AnimClass(TEXT("/Script/Engine.AnimBlueprint'/Game/MyBlueprint/Animation/MyAnimBP.MyAnimBP_C'"));
	if (AnimClass.Succeeded())
	{
		UE_LOG(LogTemp, Log, TEXT("AnimClassSucceeded"));
		GetMesh()->SetAnimClass(AnimClass.Class);
	}
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	TSubclassOf<UUserWidget> MyPlayerScreenClass = LoadClass<UUserWidget>(nullptr, TEXT("WidgetBlueprint'/Game/MyBlueprint/WBP_PlayerScreen.WBP_PlayerScreen_C'"));

	//MyPlayerScreen 은 TSubclassOf<UUserWidget> 이며, 위젯블루프린트 클래스이고 이걸 객체화하여 화면에 붙이는 과정이다
	if (IsValid(MyPlayerScreenClass))
	{
		/*MyChracter 의 GetController 가 PlayerController 인지확인해주면서
		추후 PlayerController 뿐만아니라 다른용도의 재사용성도 고려하면서, 
		 안정성을 부여함 
		*/
		APlayerController* PC = Cast<APlayerController>(GetController());
		if (IsValid(PC))
		{
			MyPlayerScreenInstance = CreateWidget<UUserWidget>(PC, MyPlayerScreenClass);
		}

		if (IsValid(MyPlayerScreenInstance))
		{
			MyPlayerScreenInstance->AddToViewport();
		}
	}

	GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

void AMyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}	

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APlayerController* MyController = Cast<APlayerController>(Controller);
	if (!MyController) {
		return;
	}

	// 현재 액터의 Yaw 회전과 컨트롤러의 Yaw 회전을 가져옵니다.
	// FMath::UnwindDegrees 는 -180도에서 180 도로 각도를 정규화 시켜서 0도에서 360으로 표기되는경우의수를 없앱니다.
	float CurrentYaw = FMath::UnwindDegrees(GetActorRotation().Yaw);
	float TargetYaw = FMath::UnwindDegrees(MyController->GetControlRotation().Yaw);

	// 가독성을 위한 람다식 : 여러 키 중 하나라도 눌려있는지 확인
	auto IsAnyMovementKeyPressed = [MyController]() -> bool {
		return MyController->IsInputKeyDown(EKeys::W) ||
			MyController->IsInputKeyDown(EKeys::A) ||
			MyController->IsInputKeyDown(EKeys::S) ||
			MyController->IsInputKeyDown(EKeys::D);
		};

	// 실제로 회전이 변경되었고, 이동 키가 눌려있는 경우에만 회전 업데이트
	if (CurrentYaw != TargetYaw && IsAnyMovementKeyPressed())
	{
		// 두 Yaw 값 사이의 차이를 계산합니다. FindDeltaAngleDegrees 값을 계산함으로써 캐릭터 회전시 반대로 회전하는 현상을 예방합니다.
		float YawDifference = FMath::FindDeltaAngleDegrees(CurrentYaw, TargetYaw);

		FRotator NewRotation = GetActorRotation();
		float NewRotationYaw = FMath::FInterpTo(CurrentYaw, CurrentYaw + YawDifference, DeltaTime, 10.0f); //부드럽게 회전합니다.
		NewRotation.Yaw = NewRotationYaw;
		SetActorRotation(NewRotation);
	}


	FVector ActorVelocity = GetMovementComponent()->Velocity; //월드 좌표계에서 character 의 Velocity
	FVector ActorLocalVelocity = GetActorRotation().UnrotateVector(ActorVelocity); //현 시점 Camera 에서 character 의 Velocity

	GetCharacterMovement()->MaxWalkSpeed = 600.f;

	if (ActorLocalVelocity.X > 0 && MyController->IsInputKeyDown(EKeys::LeftShift)) //캐릭터가 전방으로 움직이고, Shift가 눌러져 있다면
	{
		GetCharacterMovement()->MaxWalkSpeed = 1200.f; //캐릭터 최대속도 증가
	}
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

	//언리얼 기본 제공 함수 Jump 를 그대로 사용함.
	PlayerInputComponent->BindAction(TEXT("LeftClick"), EInputEvent::IE_Pressed, this, &AMyCharacter::doLeftClick);
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
	//카메라 암의 길이 설정
	float NewCameraArmLength = MyCameraSpringArm->TargetArmLength + val * 5;
	NewCameraArmLength = FMath::Min(NewCameraArmLength, 800.f);
	NewCameraArmLength = FMath::Max(NewCameraArmLength, 200.f);
	MyCameraSpringArm->TargetArmLength = NewCameraArmLength;
}

void AMyCharacter::doLeftClick()
{
	UAnimInstance* MyAnimInstance = GetMesh()->GetAnimInstance();

	MyAnimInstance->Montage_Play(MyFireMontage, 1.0f);
}
