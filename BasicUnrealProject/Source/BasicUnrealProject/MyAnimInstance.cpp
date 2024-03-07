// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"
#include "MyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UMyAnimInstance::UMyAnimInstance() 
	: Speed(0.0f), Horizontal(0.0f), Vertical(0.0f), ShouldMove(false), MyCharacter(nullptr)
{}

void UMyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UMyAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	APawn* PawnOwenerTmp = TryGetPawnOwner();

	if (IsValid(PawnOwenerTmp))
	{
		MyCharacter = Cast<AMyCharacter>(PawnOwenerTmp);
	}
}

void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (IsValid(MyCharacter))
	{
		UCharacterMovementComponent* CharacterMovement = MyCharacter->GetCharacterMovement();

		//Vector -> 방향값과 길이를 가지고 있음
		//Velocity -> 방향값과 속도(길이) 가지고 있음 
		FVector Velocity = CharacterMovement->Velocity; // 속력
		FRotator Rotation = MyCharacter->GetActorRotation();
		FVector UnRotateVector = Rotation.UnrotateVector(Velocity); // Vector 의 방향값을 없앰 
		Horizontal = UnRotateVector.Y; // Y축에서의 속력
		Vertical = UnRotateVector.X; // X축에서의 속력

		Speed = MyCharacter->GetVelocity().Size2D(); 

		// LOG를 찍어보니 가속도 Acceleration 은 고정적인 값으로 변화가 없었고 , 현재 속도에따라 Velocity 는 변화가 있었음
		FVector Acceleration = CharacterMovement->GetCurrentAcceleration();

		//캐릭터가 움직이는 상태인지를 체크하는 변수 조건 설정
		ShouldMove = Speed > 3.f && Acceleration != FVector::Zero();

		//LookAtRotation 을 변경하는 코드. Roll 값은 ContrllRotation 도 변화하지 않으므로 고려하지 않음
		{
			FRotator ContralRotation = MyCharacter->GetControlRotation();

			// Rotator 을 -180 에서 180 으로 정규화
			ContralRotation.Normalize(); 
			//FindDeltaAngleDegrees를 사용하는이유는 그냥 ContrallRotation 을 사용하면 Wald좌표를 가져오고 AimOffset 의 180에서 -180 도의 값에 충족하지 못하기때문이다
			float DeltaAngleRoationYaw = FMath::FindDeltaAngleDegrees(MyCharacter->GetActorRotation().Yaw, ContralRotation.Yaw);

			float TargetYaw = DeltaAngleRoationYaw; float CurrentYaw = LookAtRotation.Yaw;
			// RInterpTo 를 사용하지 않은 이유는 -180 에서 180으로 짧은 경로로 이동하기 때문에 길게 돌아가게 구현하고싶어서이다
			float InterpolatedYaw = FMath::FInterpTo(CurrentYaw, TargetYaw, DeltaSeconds, 6);

			//UE_LOG(LogTemp, Log, TEXT("Current : %f , Target : %f"), ContralRotation.Yaw, LookAtRotation.Yaw);

			LookAtRotation.Pitch = ContralRotation.Pitch;
			LookAtRotation.Yaw = InterpolatedYaw;
		}
	}
}
