#include "Damages/CDamageHandler.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Weapons/CWeaponStructures.h"

#include "Components/CAirborneComponent.h"
#include "Components/CConditionComponent.h"
#include "Components/CHealthPointComponent.h"

UCDamageHandler::UCDamageHandler()
{
	DamageMontages.Emplace(EDamageType::Normal);
	DamageMontages.Emplace(EDamageType::Strong );
	DamageMontages.Emplace(EDamageType::Down);
	DamageMontages.Emplace(EDamageType::Begin_Down);
	DamageMontages.Emplace(EDamageType::Down);
	DamageMontages.Emplace(EDamageType::Airbone);
}

void UCDamageHandler::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	CheckNull(OwnerCharacter);

	Airborne = FHelpers::GetComponent<UCAirborneComponent>(OwnerCharacter);
	Condition = FHelpers::GetComponent<UCConditionComponent>(OwnerCharacter);
	HealthPoint = FHelpers::GetComponent<UCHealthPointComponent>(OwnerCharacter);
}

void UCDamageHandler::ApplyDamage(FDamageData& InDamageData, FHitData& InHitData)
{
	CheckNull(OwnerCharacter);
	CheckNull(HealthPoint);
	CheckTrue(HealthPoint->IsDead());

	// 무적 상태 확인
	if (!!Condition && Condition->GetInvicibleCondition() == true)
		return;

	// 체력 감소 
	HealthPoint->Damage(InHitData.Power);

	// 히트 이펙트 및 사운드 처리 
	HandleHitEffect(InHitData, InDamageData.Event->bFirstHit);

	// 슈퍼아머 상태 확인
	if (!!Condition && Condition->GetSuperArmorCondition() == true)
		return;

	// 피격 애님 진행
	PlayDamageMontage(InHitData);

	// 다운 시키는 공격인지 확인
	if (!!Condition && InHitData.bDown)
		Condition->AddDownCondition();

	// 공중 & 런치 처리 
	HandleLaunch(InHitData, InDamageData.Attacker);
}

void UCDamageHandler::HandleHitEffect(FHitData& InHitData, bool bFirstHit)
{
	CheckNull(OwnerCharacter);

	InHitData.PlaySoundWave(OwnerCharacter);
	InHitData.PlayEffect(OwnerCharacter);

	if (bFirstHit)
	{
		InHitData.PlayHitStop(OwnerCharacter);
		InHitData.PlayCameraShake(OwnerCharacter);
	}
}

void UCDamageHandler::HandleLaunch(FHitData& InHitData, ACharacter* InAttacker, const bool bIsGuarding)
{
	CheckNull(OwnerCharacter);

	FVector start = OwnerCharacter->GetActorLocation();
	FVector target = InAttacker->GetActorLocation();
	FVector direction = target - start;
	direction.Normalize();

	float dirZ = 0.0f;
	if (!!Airborne && InHitData.Airial > 0)
	{
		dirZ = Airborne->Calc_AirborenValue(InHitData.Airial, InAttacker);
		if (Airborne->GetIsAirborne() == true)
			Condition->AddAirborneCondition();
	}

	// 기본 런치
	float launchStrength = InHitData.Launch;
	if (bIsGuarding == true)
		launchStrength *= 0.5f;

	FVector launchVelocity = (-direction * launchStrength) + FVector(0, 0, dirZ);
	OwnerCharacter->LaunchCharacter(launchVelocity, false, true);
}

void UCDamageHandler::PlayDamageMontage(FHitData& InHitData)
{
	CheckNull(OwnerCharacter);
	CheckFalse(DamageMontages.Num() > 0);

	if (DamageMontages.Contains(EDamageType::Normal) == false)
		return;
	UAnimMontage* montage = DamageMontages[EDamageType::Normal].Montage;
	float playRate = DamageMontages[EDamageType::Normal].PlayRate;

	bool check = false;

	// 공중 상태인 경우
	bool isAirborne = true;
	isAirborne &= !!Condition && Condition->GetAirborneCondition() == true;

	// 지상에서 다운된 상태
	bool isGroundDown = true;
	isGroundDown &= !!Condition && Condition->GetDownCondition() == true;

	// 다운 상태도 아니고 공중 상태도 아니면 지상에서 서있는 상태
	check = isAirborne == false && isGroundDown == false;
	if (check == true)
	{
		// 다운 시키는 공격이라면 다운되는 상태로 
		if (InHitData.bDown)
		{
			if (DamageMontages.Contains(EDamageType::Begin_Down))
			{
				montage = DamageMontages[EDamageType::Begin_Down].Montage;
				playRate = DamageMontages[EDamageType::Begin_Down].PlayRate;
			}
		}
	}
	else if (isAirborne == true)
	{
		if (DamageMontages.Contains(EDamageType::Airbone))
		{
			montage = DamageMontages[EDamageType::Airbone].Montage;
			playRate = DamageMontages[EDamageType::Airbone].PlayRate;
		}
	}
	else if (isGroundDown == true)
	{

		if (DamageMontages.Contains(EDamageType::Down))
		{
			montage = DamageMontages[EDamageType::Down].Montage;
			playRate = DamageMontages[EDamageType::Down].PlayRate;
		}
	}

	OwnerCharacter->PlayAnimMontage(montage, playRate);
}
