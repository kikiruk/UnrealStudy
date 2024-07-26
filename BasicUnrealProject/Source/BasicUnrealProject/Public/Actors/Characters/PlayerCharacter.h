// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "PlayerCharacter.generated.h"

//캐릭터가 공격할때 broadCast할 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnArrowFiredDelegate);


UCLASS()
class BASICUNREALPROJECT_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// 이벤트 델리게이트 선언
	UPROPERTY(BlueprintAssignable, Category = "Animation")
	FOnArrowFiredDelegate OnArrowFired;

public:
	// Sets default values for this character's properties
	APlayerCharacter();
private:
	//사용자 정의 함수
	void doMoveForward(float val);
	void doMoveRight(float val);
	void doMouseUp(float val);
	void doMouseRight(float val);
	void doChameraArmLengthSetup(float val);
	void doLeftClick();

private:
	/*오로지 Tick 함수에서만 사용할 변수입니다.다른함수에서는 사용하지 마세요
	목적Yaw 와 현재Yaw 값의 차이가 130 도 이상일때 부드럽게 목적Yaw 값으로 회전하려고 하는데,
	회전하는 도중 130도 이하가 되면 멈춰버려서 목적Yaw 에 도달하지 못하는 문제가 발생하여서
	회전을 해야한다는 bool 값을 기억해야하기때문에 지역변수로 생성함.*/
	UPROPERTY(Category = "Animation", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	bool bMustRotateForTick;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

private:
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* MyCamera;

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* MyCameraSpringArm;

	//Player 의 HP 와 죽음 등을 관리하는 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = true))
	class UHealthComponent* PlayerHealthComponent;

	//HP 바 UI
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = true))
	class UWidgetComponent* HealthBarComponent;

	//위젯블루프린트 클래스의 객체 (인스턴스) 
	UPROPERTY(Category = "UI", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UUserWidget* MyPlayerScreenInstance;

	// 블루프린트 클래스를 참조하기 위한 변수 선언 (발사용 화살)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning", meta = (AllowPrivateAccess = true))
    TSubclassOf<AActor> ArrowBlueprint;
private:
	// 이 변수는 
	UPROPERTY(Category = "Attack", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	bool bCanBeFireArrow;

public:
	void ResetCanBeFireArrowTrue();
	
protected:
	UFUNCTION()
	void OnCapsuleCompBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
