#include "Characters/CEnemy.h"
#include "Global.h"
#include "CAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/CapsuleComponent.h"

#include "Components/CAirborneComponent.h"
#include "Components/CConditionComponent.h"
#include "Components/CHealthPointComponent.h"
#include "Components/CMovementComponent.h"
#include "Weapons/CWeaponStructures.h"


ACEnemy::ACEnemy()
{
	FHelpers::CreateActorComponent<UCHealthPointComponent>(this, &HealthPoint, "HealthPoint");
	FHelpers::CreateActorComponent<UCMovementComponent>(this, &Movement, "Movement");
	FHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State");
	FHelpers::CreateActorComponent<UCAirborneComponent>(this, &Airborne, "Airborne");
	FHelpers::CreateActorComponent<UCConditionComponent>(this, &Condition, "Condition");


	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	USkeletalMesh* mesh = nullptr; // 포인터를 명시적으로 nullptr로 초기화
	FHelpers::GetAsset<USkeletalMesh>(&mesh, "/Script/Engine.SkeletalMesh'/Game/Characters/Mesh/SK_Mannequin.SK_Mannequin'");
	if(mesh != nullptr)
		GetMesh()->SetSkeletalMesh(mesh);

	TSubclassOf<UCAnimInstance> animInstance;
	FHelpers::GetClass<UCAnimInstance>(&animInstance, "/Script/Engine.AnimBlueprint'/Game/ABP_CPlayer.ABP_CPlayer_C'");
	GetMesh()->SetAnimClass(animInstance);

	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);

	FHelpers::GetAsset<UAnimMontage>(&DamagedMontage, "/Script/Engine.AnimMontage'/Game/Characters/Montages/HitReaction_Montage.HitReaction_Montage'");

	FHelpers::GetAsset<UAnimMontage>(&AirborneDamagedMontage, "/Script/Engine.AnimMontage'/Game/Characters/Montages/Damage/Airborne_Fall.Airborne_Fall'");
	FHelpers::GetAsset<UAnimMontage>(&DeadMontage, "/Script/Engine.AnimMontage'/Game/Characters/Montages/Enemy_DeadFall_Montage.Enemy_DeadFall_Montage'");
}


void ACEnemy::BeginPlay()
{
	Super::BeginPlay();

	Movement->OnWalk();

	for (int i = 0; i < GetMesh()->GetMaterials().Num(); i++)
	{
		UMaterialInterface* material = GetMesh()->GetMaterials()[i];
		GetMesh()->SetMaterial(i, UMaterialInstanceDynamic::Create(material, this));
	}

	Change_Color(OriginColor);

	State->OnStateTypeChanged.AddDynamic(this, &ACEnemy::OnStateTypeChanged);

	if(!!Condition)
	{
		OnCharacterLanded.AddDynamic(Condition, &UCConditionComponent::RemoveAirborneCondition);
	}
}

void ACEnemy::Change_Color(const FLinearColor& InColor)
{
	for (UMaterialInterface* material : GetMesh()->GetMaterials())
	{
		UMaterialInstanceDynamic* instance = Cast<UMaterialInstanceDynamic>(material);

		if (!!instance)
			instance->SetVectorParameterValue("Color", InColor);
	}
}

float ACEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Damage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	DamageData.Power = Damage;
	DamageData.Attacker = Cast<ACharacter>(EventInstigator->GetPawn());
	DamageData.Causer = DamageCauser;
	DamageData.Event = (FActionDamageEvent*)&DamageEvent;

	State->SetDamagedMode();

	return Damage;
}

void ACEnemy::RestoreColor()
{
	Change_Color(OriginColor);
}

void ACEnemy::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
	case EStateType::Damaged: Damaged(); break;
	case EStateType::Dead: Dead(); break;
	}
}

void ACEnemy::OnConditionTypeChanged(EConditionState InPrevCondition, EConditionState InNewCondition)
{
	
}

void ACEnemy::Damaged()
{
	//Apply Damage
	{
		HealthPoint->Damage(DamageData.Power);
		DamageData.Power = 0.0f;
	}

	//Change Color
	{
		Change_Color(FLinearColor::Red);

		FTimerDelegate timerDelegate;
		timerDelegate.BindUFunction(this, "RestoreColor");

		GetWorld()->GetTimerManager().SetTimer(ChangeColor_TimerHandle, timerDelegate, 0.2f, false);
	}

	if (!!DamageData.Event && !!DamageData.Event->HitData)
	{
		FHitData* hitData = DamageData.Event->HitData;
		
		Play_DamageMontage(*hitData);

		// 히트 및 효과 처리 
		{
			hitData->PlayHitStop(GetWorld());
			hitData->PlaySoundWave(this);
			hitData->PlayEffect(GetWorld(), GetActorLocation(), GetActorRotation());
		}


		if (HealthPoint->IsDead() == false)
		{
			FVector start = GetActorLocation();
			FVector target = DamageData.Attacker->GetActorLocation();
			FVector direction = target - start;
			direction.Normalize();
			
			// 공중에 띄우기
			float dirZ = 0.0f;
			if (!!Airborne)
			{
				dirZ = Airborne->GetAddedAirValue(hitData->Airial, DamageData.Attacker);
			}
			FVector LaunchVelocity = -direction * hitData->Launch + FVector(0, 0, dirZ);
			LaunchCharacter(LaunchVelocity, false, true);

			// 방향 조절
			FRotator targetRotator = UKismetMathLibrary::FindLookAtRotation(start, target);
			targetRotator.Pitch = 0;
			SetActorRotation(targetRotator);
		}

		//TODO: 상태 관련한 데이터를 따로 구성해야 할까?
		//TODO: 상태 변화에 대한 정보를 받는 처리를 따로 호출할 수 있도록 해야하지않을까?
		if(!!Condition)
		{
			if (GetCharacterMovement()->IsFalling())
				Condition->AddAirborneCondition();
			if(hitData->bDown)
				Condition->AddDownCondition();
		}
	}

	if (HealthPoint->IsDead())
	{
		State->SetDeadMode();

		return;
	}

	DamageData.Attacker = nullptr;
	DamageData.Causer = nullptr;
	DamageData.Event = nullptr;
}

void ACEnemy::Launch(const FHitData& InHitData, const bool bIsGuarding)
{
	Super::Launch(InHitData, bIsGuarding);

	FVector start = GetActorLocation();
	FVector target = DamageData.Attacker->GetActorLocation();
	FVector direction = target - start;
	direction.Normalize();

	// 기본 런치 값 
	float launchStrength = InHitData.Launch;

	if (bIsGuarding)
	{
		launchStrength *= 0.5f;
	}

	LaunchCharacter(-direction * launchStrength, false, false);

	FRotator targetRotator = UKismetMathLibrary::FindLookAtRotation(start, target);
	targetRotator.Pitch = 0;
	SetActorRotation(targetRotator);
}

void Test_Trash()
{
	//if (check)
	//{
	//	FLog::Log("Current Falling : " + FString::FromInt(check));
	//}
	/*if (check)
	{
		FLog::Log("Current Airborne  : " + FString::FromInt(check));
	*/
}

void ACEnemy::Play_DamageMontage(const FHitData& hitData)
{
	Super::Play_DamageMontage(hitData);

	UAnimMontage* montage = hitData.Montage;
	float playRate = hitData.PlayRate;

	if (montage == nullptr)
	{

		bool check = false;

		// 공중 상태일 경우 
		bool isAirborne = true;
		isAirborne &= Airborne != nullptr && Airborne->GetIsAirborne() == true;

		// 지상에 다운된 상태인  경우  ( 공중 X 땅 O)
		bool isDowned = true;
		isDowned &= Condition != nullptr && Condition->GetDownCondition() == true;

		// 다운 상태도 아니고 공중 상태도 아니면 지상에 서있는 상태
		check = isAirborne == false && isDowned == false;
		if (check == true)
		{
			// 그러면 다운 시작 애니메이션 재생
			if (hitData.bDown)
				montage = DownBeginMontage;
			else
			{
				montage = DamagedMontage;
				playRate = 1.5f;
			}
		}
		else if (isAirborne == true)
		{
			// 공중 상태에서 맞는 애니메이션 처리
			montage = AirborneDamagedMontage;
			playRate = 1.5f;
		}
		else if(isDowned == true)
		{
			montage = DownDamgeMontage;
		}
	}

	PlayAnimMontage(montage, playRate);
}

void ACEnemy::End_Damaged()
{
	State->SetIdleMode();

	if (OnCharacterEndDamaged.IsBound())
		OnCharacterEndDamaged.Broadcast();
}

void ACEnemy::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	/*CheckNull(Airborne);
	Airborne->Landed(Hit);*/

	if(bShouldCountDownOnLand)
		StartDownTimer();
}

void ACEnemy::Dead()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PlayAnimMontage(DeadMontage);
}

void ACEnemy::End_Dead()
{
	Destroy();
}





////////////////////////////////////////////////////////////////////////////////////////

/// <summary>
/// Airborne Conition
/// </summary>
void ACEnemy::OnAirborneConditionActivated()
{
	// 딱히 뭐 할 건 없지만 상태 변수를 조진다
	bShouldCountDownOnLand = true;

}

void ACEnemy::OnAirborneConditionDeactivated()
{

}

/// <summary>
/// Down Condition 
/// </summary>

void ACEnemy::StartDownTimer()
{

	UCapsuleComponent* capsule= GetCapsuleComponent();
	if (capsule)
	{
		//capsule->SetCapsuleHalfHeight(40.0f); // 크기 축소
		//capsule->SetCapsuleRadius(20.0f); // 반지름 축소
	}

	if (!!State)
		State->SetDownMode();

	FTimerDelegate timerDelegate;
	timerDelegate.BindLambda([this]()
	{
		if(!!Condition)
			Condition->RemoveDownCondition();
	});

	GetWorld()->GetTimerManager().SetTimer(ChangeConditionHandle, timerDelegate, 5.0f, false);
}

void ACEnemy::OnDownConditionActivated() 
{
	CheckNull(Condition);
	check(Condition != nullptr);

	// 공중 상태라면 다운 상태에 관한 로직을 바로 하지 않고 델리게이트에 맞겨놓는다.
	if(Condition->GetAirborneCondition())
	{
		bShouldCountDownOnLand = true; 
		FLog::Log("Target has Airborne : " + GetName());

		return; 
	}

	StartDownTimer();

	if (OnCharacterDowned.IsBound())
		OnCharacterDowned.Broadcast();
}

void ACEnemy::OnDownConditionDeactivated() 
{
	// 다운 상태 풀어볼려했는데 그럴만한 여건이 안되면? 수행안함
	if(GetCharacterMovement()->IsFalling())
	{
		// 착지될 때 시점에 맡긴ㄷㅏ.

		return; 
	}

	UCapsuleComponent* capsule = GetCapsuleComponent();
    if (capsule)
    {
		//capsule->SetCapsuleHalfHeight(88.0f); // 기본 크기로 복구
		//capsule->SetCapsuleRadius(34.0f); // 기본 반지름으로 복구
    }

	// 일어나는 애님 진행 - 이 애니메이션에서 상태 바꿈 
	PlayAnimMontage(RaiseMontage);


	bShouldCountDownOnLand = false;
	if (OnCharacterRaised.IsBound())
		OnCharacterRaised.Broadcast();
}

