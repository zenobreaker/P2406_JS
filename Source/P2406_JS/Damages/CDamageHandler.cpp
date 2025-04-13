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
	FHelpers::GetAsset<UFXSystemAsset>(&CriticalEffect, "/Script/Niagara.NiagaraSystem'/Game/Assets/FX/FxER_StylizedSlash/Niagara/Stylize/NS_sm05_Stylized_Slash_01_R.NS_sm05_Stylized_Slash_01_R'");
	FHelpers::GetAsset<USoundWave>(&CriticalSound, "/Script/Engine.SoundWave'/Game/Assets/Sounds/Magic_Sci_Fi_Sword/WAV/Magic_Sword_Broken_01.Magic_Sword_Broken_01'");

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

float UCDamageHandler::CalcFinalDamage(const float InDamage, const FHitData& InHitData, AActor* InAttacker, AActor* InReceiver)
{
	float power = InDamage; 

	if (InReceiver != nullptr)
	{
		UCStatComponent* receiveStat = FHelpers::GetComponent<UCStatComponent>(InReceiver);
		if (receiveStat != nullptr)
		{
			float defense = receiveStat->GetStatValue(ECharStatType::Defense);
			power = FMath::Max(power - defense, 1.0f);
		}
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

	// 무적 상태 확인
	if (!!Condition && Condition->GetInvicibleCondition() == true)
		return;
	
	
	FHitData* hitData = InDamageData.Event->HitData;

	float finalDamage = CalcFinalDamage(InDamageData.Power, *hitData, InDamageData.Attacker, OwnerCharacter);

	// 체력 감소 
	HealthPoint->Damage(finalDamage);

	// 히트 이펙트 및 사운드 처리 
	HandleHitEffect(*hitData, InDamageData);

	// 슈퍼아머 상태 확인
	if (!!Condition && Condition->GetSuperArmorCondition() == true)
		return;

	// 피격 애님 진행
	PlayDamageMontage(*hitData);

	// 다운 시키는 공격인지 확인
	if (!!Condition && hitData->bDown)
		Condition->AddDownCondition();

	// 공중 & 런치 처리 
	HandleLaunch(*hitData, InDamageData.Attacker);
}


void UCDamageHandler::HandleHitEffect(FHitData& InHitData, const FDamageData& InDamageData)
{
	CheckNull(GetOwner());

	InHitData.PlaySoundWave(GetOwner());
	InHitData.PlayEffect(GetOwner());

	if (InDamageData.Event->bCriticalHit)
	{
		FVector location = GetOwner()->GetActorLocation();
		FRotator rotator = GetOwner()->GetActorRotation();
		rotator += EffectRotation;
		location += rotator.RotateVector(EffectLocation);

		FTransform transform;
		transform.SetLocation(location);
		transform.SetScale3D(EffectScale);

		UGameplayStatics::SpawnSoundAtLocation(GetOwner()->GetWorld(), CriticalSound, location);
		FHelpers::PlayEffect(GetOwner()->GetWorld(), CriticalEffect, transform);
	}

	if (InDamageData.Event->bFirstHit)
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

	// 기본 런치
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
