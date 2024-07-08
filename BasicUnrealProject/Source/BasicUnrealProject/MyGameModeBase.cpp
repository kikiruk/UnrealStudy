// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"

AMyGameModeBase::AMyGameModeBase()
{
	//블루프린트에서 Default Pawn Class 를 설정하지않고 C++ 에서 직접 배정하기
	//하지만 지금 GameMode 는 AMyGameModeBase 를 상속받는 블루프린트로 하기 때문에 필요없음 
	//DefaultPawnClass = AMyCharacter::StaticClass();
}