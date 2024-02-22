// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"
#include "MyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

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

		//Vector -> ���Ⱚ�� ���̸� ������ ����
		//Velocity -> ���Ⱚ�� �ӵ�(����) ������ ���� 
		FVector Velocity = MyCharacter->GetVelocity();
		FRotator ActorRotation = UKismetMathLibrary::MakeRotFromX(Velocity);
		float ActorYaw = ActorRotation.Yaw;
		float CharacterYaw = MyCharacter->GetControlRotation().Yaw;

		//UE_LOG(LogTemp, Log, TEXT("Velocity X : %f Y : %f Z : %f"), Rotation.UnrotateVector(Velocity).X, Rotation.UnrotateVector(Velocity).Y, Rotation.UnrotateVector(Velocity).Z);
		YawOffset = FMath::FindDeltaAngleDegrees(CharacterYaw, ActorYaw);

		// 2D ��ǥ�迡���� Vector �� ���̸� ����
		//  Size�� 3D ����, 2D�� XY ��ǥ ������ ����.
		// LOG�� ���� ���ӵ� Acceleration �� �������� ������ ��ȭ�� ������ , ���� �ӵ������� Velocity �� ��ȭ�� �־���
		Speed = MyCharacter->GetVelocity().Size2D(); 
		FVector Acceleration = CharacterMovement->GetCurrentAcceleration();

		//UE_LOG(LogTemp, Log, TEXT("Acceleration X : %f, Y : %f, Z : %f"), Acceleration.X, Acceleration.Y, Acceleration.Z);
		//ĳ���Ͱ� �����̴� ���������� üũ�ϴ� ���� ���� ����
		ShouldMove = Speed > 3.f && Acceleration != FVector::Zero();
	}


	//UE_LOG(LogTemp, Log, TEXT("DeltaSeconds : %f"), DeltaSeconds);
}
