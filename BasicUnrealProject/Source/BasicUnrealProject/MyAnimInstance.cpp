// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UMyAnimInstance::UMyAnimInstance() 
	: Speed(0.0f), Horizontal(0.0f), Vertical(0.0f), ShouldMove(false), PlayerCharacter(nullptr), 
	MyFireMontage(nullptr)
{
	//Fire ��Ÿ�� �ʱ�ȭ �ڵ�
	ConstructorHelpers::FObjectFinder<UAnimMontage> FireAnimMontageConstructer(TEXT(
		"'/Game/ParagonSparrow/Characters/Heroes/Sparrow/Animations/Primary_Fire_Med_Montage.Primary_Fire_Med_Montage'"));
	if (FireAnimMontageConstructer.Succeeded())
	{
		MyFireMontage = FireAnimMontageConstructer.Object;
	}
}

void UMyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UMyAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	// ĳ���� �ν��Ͻ��� ������
	PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
	if (PlayerCharacter)
	{
		// ĳ������ �̺�Ʈ�� ���ε�
		PlayerCharacter->OnArrowFired.AddDynamic(this, &UMyAnimInstance::FireMontagePlay_BindToCharacterEvents);
	}
}

void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (IsValid(PlayerCharacter))
	{
		UCharacterMovementComponent* CharacterMovement = PlayerCharacter->GetCharacterMovement();

		//Vector -> ���Ⱚ�� ���̸� ������ ����
		//Velocity -> ���Ⱚ�� �ӵ�(����) ������ ���� 
		FVector Velocity = CharacterMovement->Velocity; // �ӷ�
		FRotator Rotation = PlayerCharacter->GetActorRotation();
		FVector UnRotateVector = Rotation.UnrotateVector(Velocity); // Vector �� ���Ⱚ�� ���� 
		Horizontal = UnRotateVector.Y; // Y�࿡���� �ӷ�
		Vertical = UnRotateVector.X; // X�࿡���� �ӷ�

		Speed = PlayerCharacter->GetVelocity().Size2D(); 

		// LOG�� ���� ���ӵ� Acceleration �� �������� ������ ��ȭ�� ������ , ���� �ӵ������� Velocity �� ��ȭ�� �־���
		FVector Acceleration = CharacterMovement->GetCurrentAcceleration();

		//ĳ���Ͱ� �����̴� ���������� üũ�ϴ� ���� ���� ����
		ShouldMove = Speed > 3.f && Acceleration != FVector::Zero();

		//LookAtRotation �� �����ϴ� �ڵ�. ContrllRotation �� Roll ���� ��ȭ���� �����Ƿ� ������� ����
		{
			FRotator ContralRotation = PlayerCharacter->GetControlRotation();

			// `FMath::FindDeltaAngleDegrees`�� ĳ������ ����� ��Ʈ�ѷ��� ���� ���̸� ����Ͽ�, -180~180�� ������ ������ ��ȯ�Ѵ�.
			float DeltaAngleRoationYaw = FMath::FindDeltaAngleDegrees(PlayerCharacter->GetActorRotation().Yaw, ContralRotation.Yaw);

			float TargetYaw = DeltaAngleRoationYaw; float CurrentYaw = LookAtRotation.Yaw;
			// RInterpTo �� ������� ���� ������ -180 ���� 180���� ª�� ��η� �̵��ϱ� ������ ��� ���ư��� �����ϰ�;�̴�
			float InterpolatedYaw = FMath::FInterpTo(CurrentYaw, TargetYaw, DeltaSeconds, 6);

			// Rotator �� -180 ���� 180 ���� ����ȭ
			ContralRotation.Normalize(); 
			LookAtRotation.Pitch = ContralRotation.Pitch;
			LookAtRotation.Yaw = InterpolatedYaw;
		}
	}
}

void UMyAnimInstance::FireMontagePlay_BindToCharacterEvents()
{
	if (MyFireMontage && !Montage_IsPlaying(MyFireMontage))
	{
		Montage_Play(MyFireMontage);
	}
}

