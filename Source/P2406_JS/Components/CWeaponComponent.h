#pragma once

#include "CoreMinimal.h"
#include "Characters/CBaseCharacter.h"
#include "Components/ActorComponent.h"
#include "CWeaponComponent.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Fist, Sword, Hammer, Warp, Bow, Max,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponTypeChanged, EWeaponType, InPrevType, EWeaponType, InNewType);

UCLASS(  )
class P2406_JS_API UCWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "DataAsset")
	class UCWeaponAsset* DataAssets[(int32)EWeaponType::Max];

public:
	FORCEINLINE EWeaponType GetType() { return Type; }
	FORCEINLINE bool IsUnarmedMode() { return Type == EWeaponType::Max; }

public:
	class ACAttachment* GetAttachment();
	class UCEquipment* GetEquipment();
	class UCDoAction* GetDoAction();
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
	void DoAction();

	void ExecuteSkill(const int32 InIndex); 

public:
	void SubAction_Pressed();
	void SubAction_Released();

public:
	bool TryGuard(ACBaseCharacter::FDamageData& DamageData);

private:
	void SetMode(EWeaponType InType);
	void ChangeType(EWeaponType InType);

public:
	FWeaponTypeChanged OnWeaponTypeChanged;

private:
	EWeaponType Type = EWeaponType::Max;


private:
	class ACharacter* OwnerCharacter;
	class UCSkillComponent* Skill;

private:
	UPROPERTY()
	class UCWeaponData* Datas[(int32)EWeaponType::Max];
		
};
