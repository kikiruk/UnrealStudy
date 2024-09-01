// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/AnimNotify_PlayerFire.h"
#include "Actors/Characters/PlayerCharacter.h"

void UAnimNotify_PlayerFire::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	APlayerCharacter* character = Cast<APlayerCharacter>(MeshComp->GetOwner());

	if (character)
	{
		//������ ������ �� �� ���·� �ִϸ��̼��� �Ǿ, Character �� �ٽ� ���� ������ ���·� bool ���� True �� ����� �ڵ� 
		character->ResetCanBeFireArrowTrue();
	}
}
