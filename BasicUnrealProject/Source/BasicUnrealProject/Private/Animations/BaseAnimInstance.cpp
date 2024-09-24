// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/BaseAnimInstance.h"
#include "Actors/Characters/BaseCharacter.h"

void UBaseAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	APawn* owendPawn = TryGetPawnOwner();
	if (!owendPawn)
	{
		UE_LOG(LogTemp, Error, TEXT(" UBaseAnimInstance::NativeBeginPlay() : TryGetPawnOwner() is NULL"));
		return;
	}

	OwendBaseCharacter = Cast<ABaseCharacter>(owendPawn);
	if (!OwendBaseCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT(" UBaseAnimInstance::NativeBeginPlay() : Cast<ABaseCharacter>(owendPawn) is NULL"));
		return;
	}

	OwendBaseCharacter->OnStatesChanged.AddUObject(this, &UBaseAnimInstance::OnStatesChanged);
}

void UBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}
