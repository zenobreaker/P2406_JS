#pragma once

#include "CoreMinimal.h"
#include "Characters/CBaseCharacter.h"
#include "Characters/IStatable.h"
#include "Characters/IDamagable.h"
#include "Characters/Condition_Interfaces/IDownable.h"
#include "Components/CStateComponent.h"
#include "CEnemy.generated.h"



UCLASS()
class P2406_JS_API ACEnemy 
	: public ACBaseCharacter
	, public IIStatable
	, public IIDamagable
	, public IIDownable

{
	GENERATED_BODY()

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

protected:
	UPROPERTY(VisibleAnywhere)
	class UCAirborneComponent* Airborne;

protected:
	UPROPERTY(VisibleAnywhere)
	class UCConditionComponent* Condition; 

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
	
	UFUNCTION()
	void OnConditionTypeChanged(EConditionState InPrevCondition, EConditionState InNewCondition);

protected:
	virtual void Damaged();

public:
	virtual void End_Damaged() override;

private:
	void Landed(const FHitResult& Hit) override;
	void Dead();

public:
	void End_Dead() override;

private:
	FTimerHandle ChangeColor_TimerHandle;


public: 
	virtual void OnDownConditionActivated() override;
	virtual void OnDownConditionDeactivated() override; 

};

