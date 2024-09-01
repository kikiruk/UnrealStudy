// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/AnimNotify_PlayerFire.h"
#include "Actors/Characters/PlayerCharacter.h"

void UAnimNotify_PlayerFire::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	APlayerCharacter* character = Cast<APlayerCharacter>(MeshComp->GetOwner());

	if (character)
	{
		//장전이 완전히 다 된 형태로 애니메이션이 되어서, Character 를 다시 공격 가능한 상태로 bool 값을 True 로 만드는 코드 
		character->ResetCanBeFireArrowTrue();
	}
}
