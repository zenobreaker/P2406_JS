#include "Characters/CEnemy.h"
#include "Global.h"
#include "CAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/WidgetComponent.h"

#include "Components/CapsuleComponent.h"
#include "Components/CAirborneComponent.h"
#include "Components/CConditionComponent.h"
#include "Components/CHealthPointComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CFeetComponent.h"
#include "Damages/CDamageHandler.h"
#include "Weapons/CWeaponStructures.h"

#include "Widgets/CUserWidget_Enemy.h"

ACEnemy::ACEnemy()
{
	FHelpers::CreateActorComponent<UCHealthPointComponent>(this, &HealthPoint, "HealthPoint");
	FHelpers::CreateActorComponent<UCMovementComponent>(this, &Movement, "Movement");
	FHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State");
	FHelpers::CreateActorComponent<UCAirborneComponent>(this, &Airborne, "Airborne");
	FHelpers::CreateActorComponent<UCConditionComponent>(this, &Condition, "Condition");
	FHelpers::CreateActorComponent<UCDamageHandler>(this, &DamageHandler, "Damage");

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	USkeletalMesh* mesh = nullptr; // 포인터를 명시적으로 nullptr로 초기화
	FHelpers::GetAsset<USkeletalMesh>(&mesh, "/Script/Engine.SkeletalMesh'/Game/Characters/Mesh/SK_Mannequin.SK_Mannequin'");
	if (mesh != nullptr)
		GetMesh()->SetSkeletalMesh(mesh);

	TSubclassOf<UCAnimInstance> animInstance;
	FHelpers::GetClass<UCAnimInstance>(&animInstance, "/Script/Engine.AnimBlueprint'/Game/ABP_CPlayer.ABP_CPlayer_C'");
	GetMesh()->SetAnimClass(animInstance);

	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);

	FHelpers::GetAsset<UAnimMontage>(&DamagedMontage, "/Script/Engine.AnimMontage'/Game/Characters/Montages/HitReaction_Montage.HitReaction_Montage'");

	FHelpers::GetAsset<UAnimMontage>(&AirborneDamagedMontage, "/Script/Engine.AnimMontage'/Game/Characters/Montages/Damage/Airborne_Fall.Airborne_Fall'");
	FHelpers::GetAsset<UAnimMontage>(&DeadMontage, "/Script/Engine.AnimMontage'/Game/Characters/Montages/Enemy_DeadFall_Montage.Enemy_DeadFall_Montage'");
	

	//Widget
	TSubclassOf<UCUserWidget_Enemy> labelClass;
	FHelpers::GetClass(&labelClass, "/Script/UMGEditor.WidgetBlueprint'/Game/Widgets/WB_Enemy.WB_Enemy_C'");


	FHelpers::CreateComponent<UWidgetComponent>(this, &LabelWidget, "Label", GetMesh());

	LabelWidget->SetWidgetClass(labelClass);
	LabelWidget->SetRelativeLocation(FVector(0, 0, 130));
	LabelWidget->SetDrawAtDesiredSize(true);
	//LabelWidget->SetDrawSize(FVector2D(100, 60));
	LabelWidget->SetWidgetSpace(EWidgetSpace::World);
	LabelWidget->SetTwoSided(true);
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

	if (!!LabelWidget)
	{
		LabelWidget->InitWidget();
		LabelWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		LabelWidget->SetWidgetSpace(EWidgetSpace::Screen);
		LabelWidget->CastShadow = false;

		UCUserWidget_Enemy* enemyLabel = Cast<UCUserWidget_Enemy>(LabelWidget->GetUserWidgetObject());

		enemyLabel->SetVisibility(ESlateVisibility::Hidden);
		enemyLabel->UpdateHealth(HealthPoint->GetHealth(), HealthPoint->GetMaxHealth());
		enemyLabel->UpdateName(GetName());
		enemyLabel->UpdateControllerName(GetController()->GetName());
	}

}

void ACEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); 

	CheckNull(LabelWidget);
	FTransform transform = LabelWidget->GetComponentTransform();

	APlayerCameraManager* cameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	FVector cameraLocation = cameraManager->GetTransformComponent()->GetComponentLocation();

	FRotator rotator = UKismetMathLibrary::FindLookAtRotation(transform.GetLocation(), cameraLocation);
	//rotator.Pitch = 0; // 위아래 회전 제거해서 안정감 있게
	rotator.Roll = 0;
	LabelWidget->SetWorldRotation(rotator);

	Tick_LabelRenderScale();
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
	DamageData.Event = ( b*)&DamageEvent;

	State->SetDamagedMode();

	return Damage;
}

void ACEnemy::RestoreColor()
{
	Change_Color(OriginColor);
}

void ACEnemy::OnHealthPointChanged(float InHealth, float InMaxHealth)
{
	CheckNull(LabelWidget);
	UCUserWidget_Enemy* enemyLabel = Cast<UCUserWidget_Enemy>(LabelWidget->GetUserWidgetObject());
	
	CheckNull(enemyLabel);
	enemyLabel->SetVisibility(ESlateVisibility::Visible);
	enemyLabel->UpdateHealth(HealthPoint->GetHealth(), HealthPoint->GetMaxHealth());
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

void ACEnemy::OnToggleEnemyUI(bool InToggle)
{
	CheckNull(LabelWidget);

	LabelWidget->SetVisibility(InToggle);
}

void ACEnemy::Damaged()
{
	//Apply Damage
	//{
	//	HealthPoint->Damage(DamageData.Power);
	//	DamageData.Power = 0.0f;
	//}

	if (HealthPoint->IsDead())
		return;


	//Change Color
	{
		Change_Color(FLinearColor::Red);

		FTimerDelegate timerDelegate;
		timerDelegate.BindUFunction(this, "RestoreColor");

		GetWorld()->GetTimerManager().SetTimer(ChangeColor_TimerHandle, timerDelegate, 0.2f, false);
	}

	if (!!DamageHandler)
	{
		if (!!DamageData.Event && !!DamageData.Event->HitData)
		{
			//FHitData* hitData = DamageData.Event->HitData;
			DamageHandler->ApplyDamage(DamageData);
		}
	}

	if (HealthPoint->IsDead())
	{
		State->SetDeadMode();

		return;
	}


	Tags.Remove(FName("HitByWeapon"));
	Tags.Remove(FName("NormalTrace"));

	DamageData.Attacker = nullptr;
	DamageData.Causer = nullptr;
	DamageData.Event = nullptr;

	DYNAMIC_EVENT_CALL(OnCharacterDamaged);
	// 슈퍼아머는 경직 모션을 걸리지 않기 때문에 바로 end 처리한다.
	if (!!Condition && Condition->GetSuperArmorCondition())
		End_Damaged();
}

void ACEnemy::Launch(const FHitData& InHitData, const bool bIsGuarding)
{
	Super::Launch(InHitData, bIsGuarding);

	FVector start = GetActorLocation();
	FVector target = DamageData.Attacker->GetActorLocation();
	FVector direction = target - start;
	direction.Normalize();

	// 공중에 띄우기
	float dirZ = 0.0f;
	if (!!Airborne)
	{
		//dirZ = InHitData.Airial;
		// 공중에 뜨면 공격자 충돌 무시
		if (InHitData.Airial > 0)
		{
			//GetCapsuleComponent()->IgnoreActorWhenMoving(DamageData.Attacker, true);
			CollsionEnabledType = GetCapsuleComponent()->GetCollisionEnabled();
			GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
		dirZ = Airborne->Calc_AirborneValue(InHitData.Airial, DamageData.Attacker);
		if (Airborne->GetIsAirborne())
			Condition->AddAirborneCondition();
	}

	// 기본 런치 값 
	float launchStrength = InHitData.Launch;
	if (bIsGuarding)
	{
		launchStrength *= 0.5f;
	}
	
	FVector launchVelocity = (-direction * launchStrength);
	if (InHitData.Airial > 0)
		launchVelocity = (-direction * launchStrength) + FVector(0, 0, dirZ);
	else
		launchVelocity.Z = 0.0f; 

	FLog::Print("Launch + "+ launchVelocity.ToString(), 6986);
	LaunchCharacter(launchVelocity, false, true);

	// 나 공격한 대상 바라보기 
	FRotator targetRotator = UKismetMathLibrary::FindLookAtRotation(start, target);
	targetRotator.Pitch = 0;
	SetActorRotation(targetRotator);
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
		isAirborne &= Condition != nullptr && Condition->GetAirborneCondition() == true;

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
		}
		else if (isDowned == true)
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

	if (bShouldCountDownOnLand)
		StartDownTimer();
}

void ACEnemy::Tick_LabelRenderScale()
{
	CheckNull(LabelWidget);
	UCUserWidget_Enemy* label = Cast<UCUserWidget_Enemy>(LabelWidget->GetUserWidgetObject());
	CheckNull(label);
	CheckNull(GetController());

	float MinLabelDistance = 100.f;
	float MinSizeRate = 0.5f;
	float MaxSizeRate = 1.0f;

	APlayerCameraManager* cameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

	FVector cameraLocation = cameraManager->GetCameraLocation();
	FVector targetLocation = GetActorLocation();

	float distance = FVector::Distance(cameraLocation, targetLocation);
	float clampedDistance = FMath::Clamp(distance, MinLabelDistance, MaxLabelDistance);
	//float sizeRate = 1.0f - (distance / MaxLabelDistance);
	float alpha = (clampedDistance - MinLabelDistance) / (MaxLabelDistance - MinLabelDistance); // 0~1
	float sizeRate = FMath::Lerp(MaxSizeRate, MinSizeRate, alpha);  // 거리가 멀어질수록 작게

	if (distance > MaxLabelDistance)
	{
		FLog::Print(" is too far ", 273535);
		label->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	// 거리 너무 가까우면 최소 비율 유지 
	if (distance < MinLabelDistance)
	{
		sizeRate = MinSizeRate;
	}
	else
	{
		// 최소 크기 비율보다 작아지는 경우 보정
		sizeRate = FMath::Max(sizeRate, MinSizeRate);
	}

	label->SetRenderScale(FVector2D(sizeRate, sizeRate));
}

void ACEnemy::Dead()
{
	Super::Dead();
	
	UCFeetComponent* feet = FHelpers::GetComponent<UCFeetComponent>(this);
	if (feet != nullptr)
	{
		FLog::Log(" No Feet ");
		feet->SetIKTrace(false);
	}

	// 이게 있으면 충돌됨 
	if (!!LabelWidget)
		LabelWidget->DestroyComponent();

	if (DeadMontage != nullptr)
		PlayAnimMontage(DeadMontage);
	else
		End_Dead(); 
}

void ACEnemy::End_Dead()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DYNAMIC_EVENT_CALL(OnCharacterEndDead);
	Destroy();
}

void ACEnemy::End_Downed()
{
	State->SetIdleMode();
	bCanAct = true;

	UCFeetComponent* feet= FHelpers::GetComponent<UCFeetComponent>(this);
	if (feet != nullptr)
	{
		FLog::Log(" Yes Feet ");
		feet->SetIKTrace(true);
	}


	if (OnCharacterRaised.IsBound())
		OnCharacterRaised.Broadcast();
}





////////////////////////////////////////////////////////////////////////////////////////

/// <summary>
/// Airborne Condition
/// </summary>
void ACEnemy::OnAirborneConditionActivated()
{
	CollsionEnabledType = GetCapsuleComponent()->GetCollisionEnabled();
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	
	bShouldCountDownOnLand = true;
	bCanAct = false;
	Condition->AddDownCondition();
}

void ACEnemy::OnAirborneConditionDeactivated()
{
	if (OnCharacterRaised.IsBound())
		OnCharacterRaised.Broadcast();
}

/// <summary>
/// Down Condition 
/// </summary>

void ACEnemy::StartDownTimer()
{

	UCapsuleComponent* capsule = GetCapsuleComponent();
	if (capsule)
	{
		//capsule->SetCapsuleHalfHeight(40.0f); // 크기 축소
		//capsule->SetCapsuleRadius(20.0f); // 반지름 축소
	}

	if (!!State)
		State->SetDownMode();

	GetWorld()->GetTimerManager().ClearTimer(ChangeConditionHandle);
	FTimerDelegate timerDelegate;
	timerDelegate.BindLambda([this]()
	{
		GetWorld()->GetTimerManager().ClearTimer(ChangeConditionHandle);
		if (!!Condition)
			Condition->RemoveDownCondition();
	});

	GetWorld()->GetTimerManager().SetTimer(ChangeConditionHandle, timerDelegate, 5.0f, false);
}

void ACEnemy::OnDownConditionActivated()
{
	CheckNull(Condition);
	CheckTrue(HealthPoint->IsDead());

	// 공중 상태라면 다운 상태에 관한 로직을 바로 하지 않고 델리게이트에 맞겨놓는다.
	if (Condition->GetAirborneCondition())
	{
		bShouldCountDownOnLand = true;
		FLog::Log("Target has Airborne : " + GetName());

		return;
	}

	StartDownTimer();
	bCanAct = false;
	UCFeetComponent* feet = FHelpers::GetComponent<UCFeetComponent>(this);
	if (feet != nullptr)
	{
		feet->SetIKTrace(false);
	}

	if (OnCharacterDowned.IsBound())
		OnCharacterDowned.Broadcast();
}

void ACEnemy::OnDownConditionDeactivated()
{
	// 다운 상태 풀어볼려했는데 그럴만한 여건이 안되면? 수행안함
	if (Condition != nullptr && Condition->GetAirborneCondition() == true)
		return;

	UCapsuleComponent* capsule = GetCapsuleComponent();
	if (capsule)
	{
		//capsule->SetCapsuleHalfHeight(88.0f); // 기본 크기로 복구
		//capsule->SetCapsuleRadius(34.0f); // 기본 반지름으로 복구
	}

	CheckTrue(HealthPoint->IsDead());
	CheckTrue(State->IsDeadMode());

	// 일어나는 애님 진행 - 이 애니메이션에서 상태 바꿈 
	bShouldCountDownOnLand = false;

	if (RaiseMontage != nullptr)
	{
		PlayAnimMontage(RaiseMontage);

		return;
	}

	End_Downed();

}

