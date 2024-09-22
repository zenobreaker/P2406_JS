#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/IStatable.h"
#include "Components/CStateComponent.h"
#include "CEnemy.generated.h"

UCLASS()
class P2406_JS_API ACEnemy 
	: public ACharacter
	, public IIStatable

{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Montage")
	class UAnimMontage* DamagedMontage;

	UPROPERTY(EditAnywhere, Category = "Montage")
	class UAnimMontage* DeadMontage;

private:
	UPROPERTY(EditAnywhere, Category = "Color")
	FLinearColor OriginColor = FLinearColor::White;

protected:
	UPROPERTY(VisibleAnywhere)
	class UCHealthPointComponent* HealthPoint;

private:
	UPROPERTY(VisibleAnywhere)
	class UCMovementComponent* Movement;

protected:
	UPROPERTY(VisibleAnywhere)
	class UCStateComponent* State;


public:
	ACEnemy();

	float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;

private:
	void Change_Color(const FLinearColor& InColor);


private:
	UFUNCTION()
	void RestoreColor();

private:
	UFUNCTION()
	void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

protected:
	virtual void Damaged();

public:
	virtual void End_Damaged() override;

private:
	void Dead();

public:
	void End_Dead() override;

private:
	FTimerHandle ChangeColor_TimerHandle;

private:
	struct FDamageData
	{
		float Power;
		class ACharacter* Attacker;
		class AActor* Causer;

		struct FActionDamageEvent* Event;
	} DamageData;
};

