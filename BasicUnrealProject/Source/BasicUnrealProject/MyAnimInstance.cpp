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
		if (IsValid(MyCharacter))
		{
			CharacterMovement = MyCharacter->GetCharacterMovement();
			UE_LOG(LogTemp, Log, TEXT("CharacterMovement Initailize Complete"));
		}
	}

	//UE_LOG(LogTemp, Log, TEXT("NativeBeginPlay"));
}

void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (IsValid(MyCharacter))
	{
		//Vector -> ���Ⱚ�� ���̸� ������ ����
		//Velocity -> ���Ⱚ�� �ӵ�(����) ������ ���� 
		FVector Velocity = CharacterMovement->Velocity; // �ӷ�
		FRotator Rotation = MyCharacter->GetActorRotation();
		FVector UnRotateVector = Rotation.UnrotateVector(Velocity); // Vector �� ���Ⱚ�� ���� 
		Vertical = UnRotateVector.X; // X�࿡���� �ӷ�
		Horizontal = UnRotateVector.Y; // Y�࿡���� �ӷ�

		// 2D ��ǥ�迡���� Vector �� ���̸� ����
		//  Size�� 3D ����, 2D�� XY ��ǥ ������ ����.
		Speed = MyCharacter->GetVelocity().Size2D(); 
		FVector Acceleration = CharacterMovement->GetCurrentAcceleration();

		UE_LOG(LogTemp, Log, TEXT("Acceleration X : %f, Y : %f, Z : %f"), Acceleration.X, Acceleration.Y, Acceleration.Z);

		//ĳ���Ͱ� �����̴� ���������� üũ�ϴ� ���� ���� ����
		ShouldMove = Speed > 3.f && Acceleration != FVector::Zero();
	}


	//UE_LOG(LogTemp, Log, TEXT("DeltaSeconds : %f"), DeltaSeconds);
}
