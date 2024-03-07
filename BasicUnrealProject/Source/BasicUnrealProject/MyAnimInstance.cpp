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

		//Vector -> ���Ⱚ�� ���̸� ������ ����
		//Velocity -> ���Ⱚ�� �ӵ�(����) ������ ���� 
		FVector Velocity = CharacterMovement->Velocity; // �ӷ�
		FRotator Rotation = MyCharacter->GetActorRotation();
		FVector UnRotateVector = Rotation.UnrotateVector(Velocity); // Vector �� ���Ⱚ�� ���� 
		Horizontal = UnRotateVector.Y; // Y�࿡���� �ӷ�
		Vertical = UnRotateVector.X; // X�࿡���� �ӷ�

		Speed = MyCharacter->GetVelocity().Size2D(); 

		// LOG�� ���� ���ӵ� Acceleration �� �������� ������ ��ȭ�� ������ , ���� �ӵ������� Velocity �� ��ȭ�� �־���
		FVector Acceleration = CharacterMovement->GetCurrentAcceleration();

		//ĳ���Ͱ� �����̴� ���������� üũ�ϴ� ���� ���� ����
		ShouldMove = Speed > 3.f && Acceleration != FVector::Zero();

		//LookAtRotation �� �����ϴ� �ڵ�. Roll ���� ContrllRotation �� ��ȭ���� �����Ƿ� ������� ����
		{
			FRotator ContralRotation = MyCharacter->GetControlRotation();

			// Rotator �� -180 ���� 180 ���� ����ȭ
			ContralRotation.Normalize(); 
			//FindDeltaAngleDegrees�� ����ϴ������� �׳� ContrallRotation �� ����ϸ� Wald��ǥ�� �������� AimOffset �� 180���� -180 ���� ���� �������� ���ϱ⶧���̴�
			float DeltaAngleRoationYaw = FMath::FindDeltaAngleDegrees(MyCharacter->GetActorRotation().Yaw, ContralRotation.Yaw);

			float TargetYaw = DeltaAngleRoationYaw; float CurrentYaw = LookAtRotation.Yaw;
			// RInterpTo �� ������� ���� ������ -180 ���� 180���� ª�� ��η� �̵��ϱ� ������ ��� ���ư��� �����ϰ�;�̴�
			float InterpolatedYaw = FMath::FInterpTo(CurrentYaw, TargetYaw, DeltaSeconds, 6);

			//UE_LOG(LogTemp, Log, TEXT("Current : %f , Target : %f"), ContralRotation.Yaw, LookAtRotation.Yaw);

			LookAtRotation.Pitch = ContralRotation.Pitch;
			LookAtRotation.Yaw = InterpolatedYaw;
		}
	}
}
