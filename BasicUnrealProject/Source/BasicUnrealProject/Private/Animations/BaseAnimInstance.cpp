// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/BaseAnimInstance.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/CharacterMovementComponent.h"

bool UBaseAnimInstance::GetIsIdle()
{
	APawn* owendPawn = TryGetPawnOwner();
	if (!owendPawn) return true;

	double velocity = owendPawn->GetMovementComponent()->Velocity.Length();
	return !(velocity > 0.0f);
}
