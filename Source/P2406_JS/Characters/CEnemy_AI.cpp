#include "Characters/CEnemy_AI.h"
#include "Global.h"

#include "Components/CWeaponComponent.h"
#include "Components/CHealthPointComponent.h"
#include "Components/CAIBehaviorComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/CGuardComponent.h"
#include "Components/CAttackTraceComponent.h"

#include "Widgets/CUserWidget_Enemy.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Weapons/CEquipment.h"

ACEnemy_AI::ACEnemy_AI()
{
	PrimaryActorTick.bCanEverTick = true;


	FHelpers::CreateComponent<UWidgetComponent>(this, &LabelWidget, "Label", GetMesh());

	FHelpers::CreateActorComponent<UCWeaponComponent>(this, &Weapon, "Weapon");
	FHelpers::CreateActorComponent<UCAIBehaviorComponent>(this, &Behavior, "Behavior");
	FHelpers::CreateActorComponent<UCAttackTraceComponent>(this, &ATrace, "ATrace");


	TSubclassOf<UCUserWidget_Enemy> labelClass;
	FHelpers::GetClass(&labelClass, "/Script/UMGEditor.WidgetBlueprint'/Game/Widgets/WB_Enemy.WB_Enemy_C'");

	LabelWidget->SetWidgetClass(labelClass);
	LabelWidget->SetRelativeLocation(FVector(0, 0, 270));
	LabelWidget->SetDrawSize(FVector2D(120, 0));
	LabelWidget->SetWidgetSpace(EWidgetSpace::Screen);

	FHelpers::GetAsset<UBehaviorTree>(&BehaviorTree, "/Script/AIModule.BehaviorTree'/Game/Enemies/Melee/BT_Melee.BT_Melee'");

	if (IIGuardable::Execute_HasGuard(this))
	{
		FHelpers::CreateActorComponent<UCGuardComponent>(this, &Guard, "Guard");
	}
}

void ACEnemy_AI::BeginPlay()
{
	Super::BeginPlay();

	HealthPoint->OnHealthPointChanged.AddDynamic(this, &ACEnemy_AI::OnHealthPointChanged);

	LabelWidget->InitWidget();

	UCUserWidget_Enemy* enemyLabel = Cast<UCUserWidget_Enemy>(LabelWidget->GetUserWidgetObject());

	enemyLabel->UpdateHealth(HealthPoint->GetHealth(), HealthPoint->GetMaxHealth());
	enemyLabel->UpdateName(GetName());
	enemyLabel->UpdateControllerName(GetController()->GetName());

	if (!!Guard)
	{
		EnemyWidget = Cast<UCUserWidget_Enemy>(LabelWidget->GetUserWidgetObject());
		CheckNull(EnemyWidget);

		REGISTER_EVENT_WITH_REPLACE(Guard, OnUpdatedGuardVisiable, EnemyWidget, UCUserWidget_Enemy::UpdateGuardGaugeVisibility);

		REGISTER_EVENT_WITH_REPLACE(Guard, OnUpdatedGuardGauge, EnemyWidget, UCUserWidget_Enemy::UpdateGuardGauge);
	}

}

void ACEnemy_AI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Tick_LabelRenderScale();
}

float ACEnemy_AI::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	DamageData.Power = Damage;
	DamageData.Attacker = Cast<ACharacter>(EventInstigator->GetPawn());
	DamageData.Causer = DamageCauser;
	DamageData.Event = (FActionDamageEvent*)&DamageEvent;

	bool bBlocking = false;
	if (State->IsGuardMode())
		bBlocking = Guard->CheckBlocking(DamageData);

	if (bBlocking)
		return Damage;

	Damage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	return Damage;
}

void ACEnemy_AI::OnHealthPointChanged(float InHealth, float InMaxHealth)
{
	UCUserWidget_Enemy* enemyLabel = Cast<UCUserWidget_Enemy>(LabelWidget->GetUserWidgetObject());
	enemyLabel->UpdateHealth(HealthPoint->GetHealth(), HealthPoint->GetMaxHealth());
}

void ACEnemy_AI::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	Super::OnStateTypeChanged(InPrevType, InNewType);

	switch (InNewType)
	{
		case EStateType::Guard:
		// 이전에 가드였다면 굳이 할 이유는 없겠지?
		if (InPrevType != InNewType)
		{
			StartGuard();
		}
		break;

		case EStateType::Damaged:
		{
			StopGuard();
		}
		break;
	}


	UCUserWidget_Enemy* enemyLabel = Cast<UCUserWidget_Enemy>(LabelWidget->GetUserWidgetObject());
	enemyLabel->UpdateStateName(StaticEnum<EStateType>()->GetNameStringByValue((int64)InNewType));

}

void ACEnemy_AI::Tick_LabelRenderScale()
{
	UCUserWidget_Enemy* label = Cast<UCUserWidget_Enemy>(LabelWidget->GetUserWidgetObject());
	CheckNull(label);


	APlayerCameraManager* cameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

	FVector cameraLocation = cameraManager->GetCameraLocation();
	FVector targetLocation = GetController()->GetTargetLocation();

	float distance = FVector::Distance(cameraLocation, targetLocation);
	float sizeRate = 1.0f - (distance / MaxLabelDistance);

	// 최소 거리와 최소 크기 비율 설정 
	float MinLabelDistance = 100.0f;  // 너무 가까운 거리에서의 기준값
	float MinSizeRate = 0.5f;         // 라벨 크기의 최소 값

	if (distance > MaxLabelDistance)
	{
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

	label->SetVisibility(ESlateVisibility::Visible);
	label->SetRenderScale(FVector2D(sizeRate, sizeRate));
}

void ACEnemy_AI::Damaged()
{
	Super::Damaged();
	CheckTrue(State->IsDeadMode());

	Behavior->SetHittedMode();
}

void ACEnemy_AI::End_Damaged()
{
	Super::End_Damaged();

	Behavior->SetWaitMode();
}

bool ACEnemy_AI::HasGuard() const
{
	return true;
}

bool ACEnemy_AI::CanGuard() const
{
	CheckNullResult(Guard, false);

	return Guard->GetCanGuard();
}

void ACEnemy_AI::StartGuard()
{
	// 가드 상태를 할 수 없다면 호출해도 의미 없게 
	CheckNull(Guard);
	CheckFalse(Guard->GetCanGuard());


	/*CheckNull(Weapon);
	bool bCheck = true;
	bCheck &= static_cast<bool>(Weapon->GetEquipment()->GetEquipped());
	bCheck &= Weapon->GetEquipment()->GetBeginEquip() == false;*/
	//State->SetGuardMode();
	//Guard->StartGuard();

}

void ACEnemy_AI::StopGuard()
{
	CheckNull(Guard);

	//State->SetIdleMode();
	//Guard->StopGuard();

	/*UCUserWidget_Enemy* enemyLabel = Cast<UCUserWidget_Enemy>(LabelWidget->GetUserWidgetObject());
	enemyLabel->UpdateGuardGaugeVisibility(false);*/
}

bool ACEnemy_AI::CheckBlocking(FDamageData& InDamagedata)
{
	CheckNullResult(Guard, false);


	//UCUserWidget_Enemy* enemyLabel = Cast<UCUserWidget_Enemy>(LabelWidget->GetUserWidgetObject());

	//enemyLabel->UpdateGuardGaugeVisibility(true);


	return true;
}

bool ACEnemy_AI::GetGuarding() const
{
	CheckNullResult(Guard, false);

	return true;
}
