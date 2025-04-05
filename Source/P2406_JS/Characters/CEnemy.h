#pragma once

#include "CoreMinimal.h"
#include "Characters/CBaseCharacter.h"
#include "Characters/IStatable.h"
#include "Characters/IDamagable.h"
#include "Characters/Condition_Interfaces/IAirborne.h"
#include "Characters/Condition_Interfaces/IDownable.h"
#include "Components/CStateComponent.h"

#include "CEnemy.generated.h"



UCLASS()
class P2406_JS_API ACEnemy
	: public ACBaseCharacter
	, public IIStatable
	, public IIDamagable
	, public IIDownable
	, public IIAirborne


{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Color")
	FLinearColor OriginColor = FLinearColor::White;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Label")
	float MaxLabelDistance = 1000.0f; 

protected:
	UPROPERTY(VisibleAnywhere)
	class UCHealthPointComponent* HealthPoint;

private:
	UPROPERTY(VisibleAnywhere)
	class UCMovementComponent* Movement;

protected:
	UPROPERTY(VisibleAnywhere)
	class UCStateComponent* State;

	UPROPERTY(VisibleAnywhere, Category = "Damage")
	class UCDamageHandler* DamageHandler;

protected:
	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* LabelWidget;


public:
	ACEnemy();

	float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;


protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override; 

private:
	void Change_Color(const FLinearColor& InColor);

private:
	UFUNCTION()
	void RestoreColor();

protected:
	UFUNCTION()
	virtual void OnHealthPointChanged(float InHealth, float InMaxHealth);

	UFUNCTION()
	virtual void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

	UFUNCTION()
	void OnConditionTypeChanged(EConditionState InPrevCondition, EConditionState InNewCondition);

public:
	UFUNCTION()
	void OnToggleEnemyUI(bool InToggle);

protected:
	virtual void Damaged();
	virtual void Launch(const FHitData& InHitData, const bool bIsGuarding = false);
	virtual void Play_DamageMontage(const FHitData& hitData) override;

public:
	virtual void End_Damaged() override;

private:
	void Landed(const FHitResult& Hit) override;

	void Tick_LabelRenderScale();

protected:
	virtual void Dead() override;

public:
	void End_Dead() override;
	void End_Downed() override;

public:
	// IIAirborne을(를) 통해 상속됨
	void OnAirborneConditionActivated() override;
	void OnAirborneConditionDeactivated() override;


	void StartDownTimer();
	virtual void OnDownConditionActivated() override;
	virtual void OnDownConditionDeactivated() override;

private:
	FTimerHandle ChangeColor_TimerHandle;
	FTimerHandle ChangeConditionHandle;

};

