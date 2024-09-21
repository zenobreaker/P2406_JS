#pragma once

#include "CoreMinimal.h"
#include "Characters/CEnemy.h"
#include "CEnemy_AI.generated.h"

UCLASS()
class P2406_JS_API ACEnemy_AI : public ACEnemy
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Label")
	float MaxLabelDistance = 1000.0f; 

private:
	UPROPERTY(VisibleAnywhere)
	class UCWeaponComponent* Weapon; 

private:
	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* LabelWidget;

public:
	ACEnemy_AI();

protected:
	virtual void BeginPlay() override; 

public:
	virtual void Tick(float DeltaTime) override; 


private:
	UFUNCTION()
	void OnHealthPointChanged(float InHealth, float InMaxHealth);

private:
	void Tick_LabelRenderScale();

protected:
	void Damaged() override; 

public:
	void End_Damaged() override; 
};
