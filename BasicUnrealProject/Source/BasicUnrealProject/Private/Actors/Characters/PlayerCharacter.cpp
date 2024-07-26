// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/Characters/PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/CharacterMovementComponent.h" // 이거 꼭 넣어야하는지 확인
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

	//2D 스크린에 HP 위젯 부착
	HealthBarComponent->SetWidgetSpace(EWidgetSpace::Screen);

	//Capsule <- CameraSpringArm <- Camera 이렇게 붙임
	MyCameraSpringArm->SetupAttachment(GetCapsuleComponent());
	MyCamera->SetupAttachment(MyCameraSpringArm);
	HealthBarComponent->SetupAttachment(GetCapsuleComponent());

	//카메라 팔 길이 400 으로 설정 하고 회전 (-35.f, 0.f, 0.f)
	MyCameraSpringArm->TargetArmLength = 400.f;

	//SocketOffset 속성을 이용하여 부착된 컴포넌트의 기본 위치를 미세 조정한다
	//Aim이 화면에 나오는 것을 캐릭터 오른쪽 어깨 위로 올리기위함이다. 학원수업내용을 따라기기위해 했지만 좋은 방법은 확실히 아니다.
	MyCameraSpringArm->SocketOffset = FVector(0.f, 120.f, 75.f);
	MyCameraSpringArm->SetRelativeRotation(FRotator(-35.f, 0.f, 0.f));
	HealthBarComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 116.0f));


	//[회전 설정]
	bUseControllerRotationYaw = false; // 캐릭터가 컨트롤러 회전을 따라 자동으로 회전하지 않게 설정
	MyCameraSpringArm->bUsePawnControlRotation = true; //SpringArm 의 회전을 Controller 의 회전값에 따라 움직이도록 설정
	MyCamera->bUsePawnControlRotation = false; // 카메라가 자체적으로 회전하도록 설정 (캐릭터의 회전을 따르지 않음)

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

	//블루프린트에서 할당해주던 Chracter 의 AnimClass를 C++ 코드로 할당
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

	//MyPlayerScreen 은 TSubclassOf<UUserWidget> 이며, 위젯블루프린트 클래스이고 이걸 객체화하여 화면에 붙이는 과정이다
	if (IsValid(MyPlayerScreenClass))
	{
		/*MyChracter 의 GetController 가 PlayerController 인지확인해주면서
		추후 PlayerController 뿐만아니라 다른용도의 재사용성도 고려하면서,
		 안정성을 부여함
		*/
		APlayerController* playerController = Cast<APlayerController>(GetController());
		if (IsValid(playerController))
		{
			MyPlayerScreenInstance = CreateWidget<UUserWidget>(playerController, MyPlayerScreenClass);
		}

		if (IsValid(MyPlayerScreenInstance))
		{
			//MyPlayerScreenInstance->AddToViewport(); // 형편없는 Aim 잠시동안 안보이게 설정
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

	//목적 회전 Rotation 초기화
	FRotator PurposeRotation = GetActorRotation();
	float PurposeRotationYaw = GetActorRotation().Yaw;

	// 실제로 회전이 변경된 경우에 회전 업데이트 
	if (!FMath::IsNearlyEqual(CurrentYaw, TargetYaw, 5.f)) // 5도 이하의 차이는 같은 값으로 간주함
	{
		//FindDeltaAngleDegrees 함수를 사용함으로써 캐릭터 회전시 반대로 회전하는 현상을 예방합니다.
		float CameraCharacterDeltaDegree = FMath::FindDeltaAngleDegrees(CurrentYaw, TargetYaw);
		bool isChracterFalling = GetCharacterMovement()->IsFalling();

		if (!isChracterFalling) //공중에 떠 있는 중이 아닐때 
		{
			if (IsAnyMovementKeyPressed()) //이동 키가 눌려있는 경우 캐릭터 전체 회전 (아닐시 상체만 회전)
			{
				PurposeRotationYaw = FMath::FInterpTo(CurrentYaw, CurrentYaw + CameraCharacterDeltaDegree, DeltaTime, 10.0f); //부드럽게 회전합니다.
			}
			// 차이가 130도 이상일경우 그 차이가 없어질때까지 회전함 그 회전해야하는지여부를 bMustRotateForTick(지역변수) 로저장함
			else if (FMath::Abs(CameraCharacterDeltaDegree) >= 130.f)
			{
				bMustRotateForTick = true;
			}

			if (bMustRotateForTick == true)
			{
				PurposeRotationYaw = FMath::FInterpTo(CurrentYaw, CurrentYaw + CameraCharacterDeltaDegree, DeltaTime, 3.0f); //부드럽게 회전합니다.
			}
		}

		PurposeRotation.Yaw = PurposeRotationYaw;
		SetActorRotation(PurposeRotation);
	}
	else if (bMustRotateForTick == true)
	{
		//최대 5도 까지 차이 날수 있으므로 Yaw 값을 Target Yaw 와 맟춰줌
		bMustRotateForTick = false;
		PurposeRotation.Yaw = TargetYaw;
		SetActorRotation(PurposeRotation);
	}

	FVector ActorVelocity = GetCharacterMovement()->Velocity; //월드 좌표계에서 character 의 Velocity
	FVector ActorLocalVelocity = GetActorRotation().UnrotateVector(ActorVelocity); //현 시점 Camera 에서 character 의 Velocity

	GetCharacterMovement()->MaxWalkSpeed = 600.f;

	if (ActorLocalVelocity.X > 0 && MyController->IsInputKeyDown(EKeys::LeftShift)) //캐릭터가 전방으로 움직이고, Shift가 눌러져 있다면
	{
		GetCharacterMovement()->MaxWalkSpeed = 1200.f; //캐릭터 최대속도 증가
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

	//언리얼 기본 제공 함수 Jump 를 그대로 사용함.
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
	//카메라 암의 길이 설정
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

		//스켈레탈 메쉬의 arrow_anchor 라는 이름을 가진 소켓을 가져온다.
		FTransform SocketTranceform = GetMesh()->GetSocketTransform(FName("arrow_anchor"));
		FVector SocketLocation = SocketTranceform.GetLocation();
		FRotator SocketRotation = SocketTranceform.GetRotation().Rotator();

		/*FActorSpawnParameters 구조체는 Unreal Engine에서 액터를 스폰(생성)할 때 사용되는 매개변수 집합을 정의
		* Owner 을 설정해줌으로써 화살이 누구에 의해 발사되었는지를 알 수 있습니다.
		* 이를 통해 화살에 의한 피해를 계산하거나 게임 로직을 결정할 때 소유자를 기반으로 특별한 규칙을 적용할 수 있습니다.
		*/
		FActorSpawnParameters params;
		params.Owner = this;

		GetWorld()->SpawnActor<AActor>(ArrowBlueprint, SocketLocation, SocketRotation, params);

		// 이벤트 델리게이트 호출
		OnArrowFired.Broadcast();
	}
}
