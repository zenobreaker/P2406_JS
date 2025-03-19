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

	// ���� ���� Ȯ��
	if (!!Condition && Condition->GetInvicibleCondition() == true)
		return;

	// ü�� ���� 
	HealthPoint->Damage(InHitData.Power);

	// ��Ʈ ����Ʈ �� ���� ó�� 
	HandleHitEffect(InHitData, InDamageData.Event->bFirstHit);

	// ���۾Ƹ� ���� Ȯ��
	if (!!Condition && Condition->GetSuperArmorCondition() == true)
		return;

	// �ǰ� �ִ� ����
	PlayDamageMontage(InHitData);

	// �ٿ� ��Ű�� �������� Ȯ��
	if (!!Condition && InHitData.bDown)
		Condition->AddDownCondition();

	// ���� & ��ġ ó�� 
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

	// �⺻ ��ġ
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

	// ���� ������ ���
	bool isAirborne = true;
	isAirborne &= !!Condition && Condition->GetAirborneCondition() == true;

	// ���󿡼� �ٿ�� ����
	bool isGroundDown = true;
	isGroundDown &= !!Condition && Condition->GetDownCondition() == true;

	// �ٿ� ���µ� �ƴϰ� ���� ���µ� �ƴϸ� ���󿡼� ���ִ� ����
	check = isAirborne == false && isGroundDown == false;
	if (check == true)
	{
		// �ٿ� ��Ű�� �����̶�� �ٿ�Ǵ� ���·� 
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
