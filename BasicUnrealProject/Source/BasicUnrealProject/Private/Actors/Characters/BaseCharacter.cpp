// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Characters/BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Actors/Components/HealthComponent.h"
#include "Actors/Widget/HealthBarUserWidget.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ABaseCharacter::ABaseCharacter() : isDie(false)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerHealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthBarComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarComponent"));
	
	ConstructorHelpers::FClassFinder<UHealthBarUserWidget> healthBarConstructer(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/MyBlueprint/WBP_HealthBarUserWidget.WBP_HealthBarUserWidget_C'"));
	if (healthBarConstructer.Succeeded())
	{
		HealthBarComponent->SetWidgetClass(healthBarConstructer.Class);
	}
	
	HealthBarComponent->SetupAttachment(GetCapsuleComponent());
	HealthBarComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 116.0f));

	//2D 스크린에 HP 위젯 부착
	HealthBarComponent->SetWidgetSpace(EWidgetSpace::Screen);
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HealthBarComponent && PlayerHealthComponent)
	{
		PlayerHealthComponent->OnDeath.AddDynamic(this, &ABaseCharacter::OnCharacterDeth);
	
		UHealthBarUserWidget* PlayerHealthBar = Cast<UHealthBarUserWidget>(HealthBarComponent->GetWidget());
		if (PlayerHealthBar)
		{
			PlayerHealthBar->SetPercent(1.0f);// 캐릭터의 체력을 풀피로 표시되게 함
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BaseCharacter : problem with HealthBarComponent or PlayerHealthComponent"));
	}
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	bool currentBIsIdle = bIsIdle;
	bIsIdle = !(GetMovementComponent()->Velocity.Length() > 0.0f);

	if (currentBIsIdle != bIsIdle)
	{
		OnStatesChanged.Broadcast();
		OnCharacterStartMoving();
	}

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ABaseCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (PlayerHealthComponent && HealthBarComponent)
	{
		PlayerHealthComponent->DecreaseHealth(ActualDamage);
		UHealthBarUserWidget* PlayerHealthBar = Cast<UHealthBarUserWidget>(HealthBarComponent->GetWidget());

		if (PlayerHealthBar)
		{
			PlayerHealthBar->SetPercent(PlayerHealthComponent->GetPercent());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BaseCharacter : problem with HealthBarComponent or PlayerHealthComponent"));
	}

	return ActualDamage;
}

void ABaseCharacter::OnCharacterDeth()
{
	BaseCharacterOnDeath.Broadcast();
	isDie = true;
}

