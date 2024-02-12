// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"

void UMyAnimInstance::NativeInitializeAnimation()
{

}

void UMyAnimInstance::NativeBeginPlay()
{

}

void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	UE_LOG(LogTemp, Log, TEXT("DeltaSeconds : %f"), DeltaSeconds);
}
