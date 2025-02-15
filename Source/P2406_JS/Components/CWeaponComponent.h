#pragma once

#include "CoreMinimal.h"
#include "Enums/FEnum.h"
#include "Components/ActorComponent.h"
#include "Characters/CBaseCharacter.h"
#include "CWeaponComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBeginDoAction);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEndedDoAction);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponTypeChanged, EWeaponType, InPrevType, EWeaponType, InNewType);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGuardValueChanged, float, InValue,
	float, InMaxValue);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class P2406_JS_API UCWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Type")
	EWeaponType InitType = EWeaponType::Max;

	UPROPERTY(EditAnywhere, Category = "DataAsset")
	class UCWeaponAsset* DataAssets[(int32)EWeaponType::Max];

public:
	FORCEINLINE EWeaponType GetType() { return Type; }
	FORCEINLINE bool IsUnarmedMode() { return Type == EWeaponType::Max; }
	FORCEINLINE bool IsFallingAttack() const { return bIsFallingAttack; }
	FORCEINLINE void  SetFallingAttack(bool InValue) { bIsFallingAttack = InValue; }

public:
	class ACAttachment* GetAttachment();
	class UCEquipment* GetEquipment();
	class UCDoAction* GetDoAction();
	class UCDoAction* GetJumpDoAction(); 
	class UCSubAction* GetSubAction();
	class UCWeaponData* GetCurrentWeaponData();

private:
	bool IsIdleMode();

public:	
	UCWeaponComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickTbype, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SetUnarmedMode();

	void SetFistMode();
	void SetSwordMode();
	void SetHammerMode();
	void SetWarpMode();
	void SetBowMode();

public:
	void InitEquip();

public:
	void DoAction();
	void DoAction_Heavy();

	UFUNCTION()
	void Begin_DoAction(); 

	UFUNCTION()
	void End_DoAction(); 

public:
	void Handle_DoAction(bool InHeavyValue = false); 
	void Handle_BeginDoAction();
	void Handle_EndDoAction();

	void ExecuteSkill(const int32 InIndex); 
	void ReleaseSkill(const int32 InIndex);

public:
	void SubAction_Pressed();
	void SubAction_Released();

public:
	bool TryGuard(ACBaseCharacter::FDamageData& DamageData);

private:
	void SetMode(EWeaponType InType);
	void ChangeType(EWeaponType InType);

public:
	void PlayFallingAttackMontage();

public:
	UFUNCTION()
	void ChangeGuardValue(float InValue, float InMaxValue);

public:
	FWeaponTypeChanged OnWeaponTypeChanged;
	FGuardValueChanged OnGuardValueChanged; 
	FBeginDoAction OnBeginDoAction; 
	FEndedDoAction OnEndedDoAction;

private:
	EWeaponType Type = EWeaponType::Max;

	bool bIsFallingAttack = false; 

private:
	class ACharacter* OwnerCharacter;
	class UCSkillComponent* SkillComp;

private:
	UPROPERTY()
	class UCWeaponData* Datas[(int32)EWeaponType::Max];
		
};
