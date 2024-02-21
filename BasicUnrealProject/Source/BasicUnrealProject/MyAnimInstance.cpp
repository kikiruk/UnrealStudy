// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"
#include "MyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UMyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	UE_LOG(LogTemp, Log, TEXT("NativeInitializeAnimation"));
}

void UMyAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	APawn* PawnOwenerTmp = TryGetPawnOwner();

	if (IsValid(PawnOwenerTmp))
	{
		MyCharacter = Cast<AMyCharacter>(PawnOwenerTmp);
	}

	//UE_LOG(LogTemp, Log, TEXT("NativeBeginPlay"));
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
		//UE_LOG(LogTemp, Log, TEXT("Velocity X : %f Y : %f Z : %f"), Velocity.X, Velocity.Y, Velocity.Z);
		FRotator Rotation = MyCharacter->GetActorRotation();
		FVector UnRotateVector = Rotation.UnrotateVector(Velocity).GetSafeNormal(); // Vector 의 방향값을 없앰 , GetSafeNormal 함수를 이용하여 정규화함

		//UE_LOG(LogTemp, Log, TEXT("Velocity X : %f Y : %f Z : %f"), Rotation.UnrotateVector(Velocity).X, Rotation.UnrotateVector(Velocity).Y, Rotation.UnrotateVector(Velocity).Z);

		Vertical = UnRotateVector.X; // X축에서의 속력
		Horizontal = UnRotateVector.Y; // Y축에서의 속력

		// 2D 좌표계에서의 Vector 의 길이를 구함
		//  Size는 3D 길이, 2D는 XY 좌표 기준의 길이.
		// LOG를 찍어보니 가속도 Acceleration 은 고정적인 값으로 변화가 없었고 , 현재 속도에따라 Velocity 는 변화가 있었음
		Speed = MyCharacter->GetVelocity().Size2D(); 
		FVector Acceleration = CharacterMovement->GetCurrentAcceleration();

		//UE_LOG(LogTemp, Log, TEXT("Acceleration X : %f, Y : %f, Z : %f"), Acceleration.X, Acceleration.Y, Acceleration.Z);
		//캐릭터가 움직이는 상태인지를 체크하는 변수 조건 설정
		ShouldMove = Speed > 3.f && Acceleration != FVector::Zero();
	}


	//UE_LOG(LogTemp, Log, TEXT("DeltaSeconds : %f"), DeltaSeconds);
}
