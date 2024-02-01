// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "MyCharacter.h"

AMyGameModeBase::AMyGameModeBase()
{
	ConstructorHelpers::FClassFinder<AMyCharacter> BP_MyCharacter(
		TEXT("/Script/Engine.Blueprint'/Game/MyBlueprint/BP_MyChracter.BP_MyChracter_C'"));
	
	if (BP_MyCharacter.Succeeded())
	{
		DefaultPawnClass = BP_MyCharacter.Class;
	}
}