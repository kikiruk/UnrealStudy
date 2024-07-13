// Fill out your copyright notice in the Description page of Project Settings.

#include "Animations/PlayerAnimInstance.h"
#include "Actors/Characters/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UPlayerAnimInstance::UPlayerAnimInstance() 
	: Speed(0.0f), Horizontal(0.0f), Vertical(0.0f), ShouldMove(false), PlayerCharacter(nullptr), 
	MyFireMontage(nullptr)
{
	//Fire 몽타주 초기화 코드
	ConstructorHelpers::FObjectFinder<UAnimMontage> FireAnimMontageConstructer(TEXT(
		"/Script/Engine.AnimMontage'/Game/MyBlueprint/Animation/Player/Primary_Fire_Med_Montage.Primary_Fire_Med_Montage'"));
	if (FireAnimMontageConstructer.Succeeded())
	{
		MyFireMontage = FireAnimMontageConstructer.Object;
	}
}

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UPlayerAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	// 캐릭터 인스턴스를 가져옴
	PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
	if (PlayerCharacter)
	{
		// 캐릭터의 이벤트에 바인딩
		PlayerCharacter->OnArrowFired.AddDynamic(this, &UPlayerAnimInstance::FireMontagePlay_BindToCharacterEvents);
	}
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (IsValid(PlayerCharacter))
	{
		UCharacterMovementComponent* CharacterMovement = PlayerCharacter->GetCharacterMovement();

		//Vector -> 방향값과 길이를 가지고 있음
		//Velocity -> 방향값과 속도(길이) 가지고 있음 
		FVector Velocity = CharacterMovement->Velocity; // 속력
		FRotator Rotation = PlayerCharacter->GetActorRotation();
		FVector UnRotateVector = Rotation.UnrotateVector(Velocity); // Vector 의 방향값을 없앰 
		Horizontal = UnRotateVector.Y; // Y축에서의 속력
		Vertical = UnRotateVector.X; // X축에서의 속력

		Speed = PlayerCharacter->GetVelocity().Size2D(); 

		// LOG를 찍어보니 가속도 Acceleration 은 고정적인 값으로 변화가 없었고 , 현재 속도에따라 Velocity 는 변화가 있었음
		FVector Acceleration = CharacterMovement->GetCurrentAcceleration();

		//캐릭터가 움직이는 상태인지를 체크하는 변수 조건 설정
		ShouldMove = Speed > 3.f && Acceleration != FVector::Zero();

		//LookAtRotation 을 변경하는 코드. ContrllRotation 의 Roll 값은 변화하지 않으므로 고려하지 않음
		{
			FRotator ContralRotation = PlayerCharacter->GetControlRotation();

			// `FMath::FindDeltaAngleDegrees`는 캐릭터의 방향과 컨트롤러의 각도 차이를 계산하여, -180~180도 사이의 값으로 반환한다.
			float DeltaAngleRoationYaw = FMath::FindDeltaAngleDegrees(PlayerCharacter->GetActorRotation().Yaw, ContralRotation.Yaw);

			float TargetYaw = DeltaAngleRoationYaw; float CurrentYaw = LookAtRotation.Yaw;
			// RInterpTo 를 사용하지 않은 이유는 -180 에서 180으로 짧은 경로로 이동하기 때문에 길게 돌아가게 구현하고싶어서이다
			float InterpolatedYaw = FMath::FInterpTo(CurrentYaw, TargetYaw, DeltaSeconds, 6);

			// Rotator 을 -180 에서 180 으로 정규화
			ContralRotation.Normalize(); 
			LookAtRotation.Pitch = ContralRotation.Pitch;
			LookAtRotation.Yaw = InterpolatedYaw;
		}
	}
}

void UPlayerAnimInstance::FireMontagePlay_BindToCharacterEvents()
{
	if (MyFireMontage && !Montage_IsPlaying(MyFireMontage))
	{
		Montage_Play(MyFireMontage);
	}
}

void UPlayerAnimInstance::AnimNotify_Test()
{
	UE_LOG(LogTemp, Warning, TEXT("AnimNotify_Test@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"));
}

