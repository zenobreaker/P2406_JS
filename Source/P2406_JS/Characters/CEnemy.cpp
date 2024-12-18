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
	CHelpers::CreateActorComponent<UCHealthPointComponent>(this, &HealthPoint, "HealthPoint");
	CHelpers::CreateActorComponent<UCMovementComponent>(this, &Movement, "Movement");
	CHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State");
	CHelpers::CreateActorComponent< UCAirborneComponent>(this, &Airborne, "Airborne");
	CHelpers::CreateActorComponent<UCConditionComponent>(this, &Condition, "Condition");


	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	USkeletalMesh* mesh = nullptr; // 포인터를 명시적으로 nullptr로 초기화
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "/Script/Engine.SkeletalMesh'/Game/Characters/Mesh/SK_Mannequin.SK_Mannequin'");
	if(mesh != nullptr)
		GetMesh()->SetSkeletalMesh(mesh);

	TSubclassOf<UCAnimInstance> animInstance;
	CHelpers::GetClass<UCAnimInstance>(&animInstance, "/Script/Engine.AnimBlueprint'/Game/ABP_CPlayer.ABP_CPlayer_C'");
	GetMesh()->SetAnimClass(animInstance);

	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);

	CHelpers::GetAsset<UAnimMontage>(&DamagedMontage, "/Script/Engine.AnimMontage'/Game/Characters/Montages/HitReaction_Montage.HitReaction_Montage'");

	CHelpers::GetAsset<UAnimMontage>(&AirborneDamagedMontage, "/Script/Engine.AnimMontage'/Game/Characters/Montages/Damage/Airborne_Fall.Airborne_Fall'");
	CHelpers::GetAsset<UAnimMontage>(&DeadMontage, "/Script/Engine.AnimMontage'/Game/Characters/Montages/Enemy_DeadFall_Montage.Enemy_DeadFall_Montage'");
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

		UAnimMontage* montage = hitData->Montage;
		float playRate = hitData->PlayRate;

		if (montage == nullptr)
		{
			bool check = false; 
			check |= GetCharacterMovement()->IsFalling() == false;
			check |= Airborne != nullptr && Airborne->GetIsAirborne() == false;
			if (check == true )
			{
				// 지상 상태에서 맞는 애니메이션 처리 
				montage = DamagedMontage;
				playRate = 1.5f;
			}
			else
			{
				// 공중 상태에서 맞는 애니메이션 처리
				montage = AirborneDamagedMontage;
				playRate = 1.5f; 
			}
		}
		
		PlayAnimMontage(montage, playRate);

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

void ACEnemy::End_Damaged()
{
	State->SetIdleMode();

	if (OnCharacterEndDamaged.IsBound())
		OnCharacterEndDamaged.Broadcast();
}

void ACEnemy::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	CheckNull(Airborne);

	Airborne->Landed(Hit);

	if (OnCharacterLandedDelegate.IsBound())
		OnCharacterLandedDelegate.Broadcast();
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
/// Down Condition 
/// </summary>
void ACEnemy::OnDownConditionActivated() 
{
	
}

void ACEnemy::OnDownConditionDeactivated() 
{
	
}
