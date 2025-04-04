#include "Damages/CDamageHandler.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Weapons/CWeaponStructures.h"

#include "Components/CAirborneComponent.h"
#include "Components/CConditionComponent.h"
#include "Components/CHealthPointComponent.h"
#include "Components/CStatComponent.h"

UCDamageHandler::UCDamageHandler()
{
	DamageMontages.Emplace(EDamageAnimType::Normal);
	DamageMontages.Emplace(EDamageAnimType::Strong);
	DamageMontages.Emplace(EDamageAnimType::Down);
	DamageMontages.Emplace(EDamageAnimType::Begin_Down);
	DamageMontages.Emplace(EDamageAnimType::Down);
	DamageMontages.Emplace(EDamageAnimType::Airbone);
}

void UCDamageHandler::BeginPlay()
{
	Super::BeginPlay();
	AActor* thisOne = GetOwner();
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	HealthPoint = FHelpers::GetComponent<UCHealthPointComponent>(GetOwner());
	
	CheckNull(OwnerCharacter);

	Airborne = FHelpers::GetComponent<UCAirborneComponent>(OwnerCharacter);
	Condition = FHelpers::GetComponent<UCConditionComponent>(OwnerCharacter);
}

void UCDamageHandler::OnComponentCreated()
{
	Super::OnComponentCreated();
	RegisterComponent();
}

float UCDamageHandler::CalcFinalDamage(const FHitData& InHitData, AActor* InAttacker, AActor* InReceiver)
{
	float power = InHitData.Power; 

	UCStatComponent* receiveStat = FHelpers::GetComponent<UCStatComponent>(InReceiver); 
	if (!!receiveStat)
	{
		float defense = receiveStat->GetStatValue(ECharStatType::Defense); 
		power = FMath::Max(power - defense, 1.0f); 
	}

	return power;
}

void UCDamageHandler::ApplyDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	DamageData.Power = Damage;
	DamageData.Attacker = Cast<ACharacter>(EventInstigator->GetPawn());
	DamageData.Causer = DamageCauser;
	DamageData.Event = (FActionDamageEvent*)&DamageEvent;

	ApplyDamage(DamageData);
}

void UCDamageHandler::ApplyDamage(FDamageData& InDamageData)
{
	CheckNull(HealthPoint);
	CheckTrue(HealthPoint->IsDead());

	// ���� ���� Ȯ��
	if (!!Condition && Condition->GetInvicibleCondition() == true)
		return;
	
	
	FHitData* hitData = InDamageData.Event->HitData;

	float finalDamage = CalcFinalDamage(*hitData, InDamageData.Attacker, OwnerCharacter);

	// ü�� ���� 
	HealthPoint->Damage(finalDamage);

	// ��Ʈ ����Ʈ �� ���� ó�� 
	HandleHitEffect(*hitData, InDamageData.Event->bFirstHit);

	// ���۾Ƹ� ���� Ȯ��
	if (!!Condition && Condition->GetSuperArmorCondition() == true)
		return;

	// �ǰ� �ִ� ����
	PlayDamageMontage(*hitData);

	// �ٿ� ��Ű�� �������� Ȯ��
	if (!!Condition && hitData->bDown)
		Condition->AddDownCondition();

	// ���� & ��ġ ó�� 
	HandleLaunch(*hitData, InDamageData.Attacker);
}


void UCDamageHandler::HandleHitEffect(FHitData& InHitData, bool bFirstHit)
{
	CheckNull(GetOwner());

	InHitData.PlaySoundWave(GetOwner());
	InHitData.PlayEffect(GetOwner());

	if (bFirstHit)
	{
		InHitData.PlayHitStop(GetOwner());
		InHitData.PlayCameraShake(GetOwner());
	}
}

void UCDamageHandler::HandleLaunch(FHitData& InHitData, ACharacter* InAttacker, const bool bIsGuarding)
{
	CheckNull(OwnerCharacter);

	FVector start = OwnerCharacter->GetActorLocation();
	FVector target = InAttacker->GetActorLocation();
	FVector direction = target - start;
	direction.Normalize();
	direction.Z = 0;

	float dirZ = 0.0f;
	if (!!Airborne && InHitData.Airial > 0)
	{
		Condition->AddAirborneCondition();
	}
	dirZ = Airborne->Calc_AirborneValue(InHitData.Airial, InAttacker);

	// �⺻ ��ġ
	float launchStrength = InHitData.Launch;
	if (bIsGuarding == true)
		launchStrength *= 0.5f;
	FVector launchVelocity = (-direction * launchStrength) + FVector(0, 0, dirZ);
	FLog::Log("Current Launch: " + FString::SanitizeFloat(InHitData.Launch));
	FLog::Log("Damage Handler Launch : " + launchVelocity.ToString());

	FRotator targetRotator = UKismetMathLibrary::FindLookAtRotation(start, target);
	targetRotator.Pitch = 0;
	OwnerCharacter->SetActorRotation(targetRotator);

	OwnerCharacter->LaunchCharacter(launchVelocity, false, true);
}

void UCDamageHandler::PlayDamageMontage(FHitData& InHitData)
{
	CheckNull(OwnerCharacter);
	CheckFalse(DamageMontages.Num() > 0);

	if (DamageMontages.Contains(EDamageAnimType::Normal) == false)
		return;
	UAnimMontage* montage = DamageMontages[EDamageAnimType::Normal].Montage;
	float playRate = DamageMontages[EDamageAnimType::Normal].PlayRate;

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
			if (DamageMontages.Contains(EDamageAnimType::Begin_Down))
			{
				montage = DamageMontages[EDamageAnimType::Begin_Down].Montage;
				playRate = DamageMontages[EDamageAnimType::Begin_Down].PlayRate;
			}
		}
	}
	else if (isAirborne == true)
	{
		if (DamageMontages.Contains(EDamageAnimType::Airbone))
		{
			montage = DamageMontages[EDamageAnimType::Airbone].Montage;
			playRate = DamageMontages[EDamageAnimType::Airbone].PlayRate;
		}
	}
	else if (isGroundDown == true)
	{

		if (DamageMontages.Contains(EDamageAnimType::Down))
		{
			montage = DamageMontages[EDamageAnimType::Down].Montage;
			playRate = DamageMontages[EDamageAnimType::Down].PlayRate;
		}
	}

	OwnerCharacter->PlayAnimMontage(montage, playRate);
}
