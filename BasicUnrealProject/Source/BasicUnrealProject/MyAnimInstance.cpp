// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"
#include "MyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

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

		
		FRotator ContralRotation = MyCharacter->GetControlRotation();
		float LookAtRotationYaw = FMath::FindDeltaAngleDegrees(ContralRotation.Yaw, Rotation.Yaw); //Yaw의 각도와 Rotation(Actor 의 Rotation) 을 빼서 각도의 차이 구하기 
		ContralRotation.Yaw = -LookAtRotationYaw; // Yaw 값이 반대로 회전하는걸 방지하기위해 - 부호 넣음 
		ContralRotation.Normalize(); // Rotator 을 -180 에서 180 으로 정규화
		LookAtRotation = ContralRotation; // LookAtRotation : 멤버변수 
	}
}
