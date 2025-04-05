#include "Characters/CEnemy_AI.h"
#include "Global.h"

#include "Components/WidgetComponent.h"

#include "Characters/CAIController.h"
#include "Components/CWeaponComponent.h"
#include "Components/CHealthPointComponent.h"
#include "Components/CAIBehaviorComponent.h"
#include "Components/CGuardComponent.h"
#include "Components/CAttackTraceComponent.h"


#include "BehaviorTree/BehaviorTree.h"
#include "Weapons/CEquipment.h"

#include "GameInstances/CGameInstance.h"
#include "GameInstances/CBattleManager.h"
#include "Widgets/CUserWidget_Enemy.h"

int32 ACEnemy_AI::GlobalID = 1;

ACEnemy_AI::ACEnemy_AI()
{
	PrimaryActorTick.bCanEverTick = true;

	AIID = GlobalID++;

	

	FHelpers::CreateActorComponent<UCWeaponComponent>(this, &Weapon, "Weapon");
	FHelpers::CreateActorComponent<UCAIBehaviorComponent>(this, &Behavior, "Behavior");
	FHelpers::CreateActorComponent<UCAttackTraceComponent>(this, &ATrace, "ATrace");



	//FHelpers::GetAsset<UBehaviorTree>(&BehaviorTree, "/Script/AIModule.BlackboardData'/Game/Enemies/Melee/Melee_Base/BB_Melee_Small.BB_Melee_Small'");

	if (IIGuardable::Execute_HasGuard(this))
	{
		FHelpers::CreateActorComponent<UCGuardComponent>(this, &Guard, "Guard");
	}
}

void ACEnemy_AI::BeginPlay()
{
	Super::BeginPlay();

	CheckNull(GetController());
	CheckNull(HealthPoint);
	HealthPoint->OnHealthPointChanged.AddDynamic(this, &ACEnemy_AI::OnHealthPointChanged);

	if (!!Guard && !!LabelWidget)
	{
		EnemyWidget = Cast<UCUserWidget_Enemy>(LabelWidget->GetUserWidgetObject());
		CheckNull(EnemyWidget);

		REGISTER_EVENT_WITH_REPLACE(Guard, OnUpdatedGuardVisiable, EnemyWidget, UCUserWidget_Enemy::UpdateGuardGaugeVisibility);

		REGISTER_EVENT_WITH_REPLACE(Guard, OnUpdatedGuardGauge, EnemyWidget, UCUserWidget_Enemy::UpdateGuardGauge);
	}

	GetController()->Possess(this);
	SetRegisterToBattleManager();
}

float ACEnemy_AI::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	DamageData.Power = Damage;
	DamageData.Attacker = Cast<ACharacter>(EventInstigator->GetPawn());
	DamageData.Causer = DamageCauser;
	DamageData.Event = (FActionDamageEvent*)&DamageEvent;

	bool bBlocking = false;
	bool bCountering = false; 
	if (Guard != nullptr && Guard->GetGuarding())
	{
		bBlocking = Guard->CheckBlocking(DamageData.Attacker);

		bCountering = Guard->GetCountering();
	}

	// 카운터 중이라면 데미지를 입지 않는다. 
	if (bCountering)
	{

		return Damage;
	}

	// 가드 됐다면 쪼금만 밀리게 
	if (bBlocking)
	{
		// 데미지 입었다는 신호 전달
		// 막히면 Damaged 함수가 콜이 안되므로 StateComp의 변화는 되지 않음
		Behavior->SetDamageMode();
		// 이미 구현된함수 호출 
		Launch(*DamageData.Event->HitData, true);

		return Damage;
	}

	Damage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	return Damage;
}

void ACEnemy_AI::Launch(const FHitData& InHitData, const bool bIsGuarding)
{
	Super::Launch(InHitData, bIsGuarding); 

	ACAIController* controller = Cast<ACAIController>(GetController());
	CheckNull(controller); 

	// Focus 일시 해제
	//controller->ClearFocus(EAIFocusPriority::Gameplay);
}

void ACEnemy_AI::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	Super::OnStateTypeChanged(InPrevType, InNewType);

	CheckNull(LabelWidget);
	CheckNull(LabelWidget->GetUserWidgetObject());

	UCUserWidget_Enemy* enemyLabel = Cast<UCUserWidget_Enemy>(LabelWidget->GetUserWidgetObject());
	enemyLabel->UpdateStateName(StaticEnum<EStateType>()->GetNameStringByValue((int64)InNewType));

}

void ACEnemy_AI::Damaged()
{
	Super::Damaged();
	CheckTrue(HealthPoint->IsDead());
	CheckNull(Behavior); 

	auto* target = Behavior->GetTarget();
	if (target != nullptr)
		Behavior->SetTarget(target);
	
	Behavior->SetDamageMode();
}

void ACEnemy_AI::Dead()
{
	Super::Dead(); 

	//State->SetDeadMode();
	Behavior->SetDeadMode();

	//UCGameInstance* instance = Cast<UCGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	//CheckNull(instance);

	//UCBattleManager* battleManager = instance->BattleManager;
	//CheckNull(battleManager);

	//battleManager->UnregistGroup(GroupID, this);
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

void ACEnemy_AI::SetRegisterToBattleManager()
{
	UCGameInstance* instance = Cast<UCGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	CheckNull(instance);

	UCBattleManager* battleManager = instance->BattleManager;
	CheckNull(battleManager);


	// 등록
	battleManager->RegistGroup(GroupID, this);

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

}

void ACEnemy_AI::StopGuard()
{
	CheckNull(Guard);
}

bool ACEnemy_AI::CheckBlocking(FDamageData& InDamagedata)
{
	CheckNullResult(Guard, false);

	return true;
}

bool ACEnemy_AI::GetGuarding() const
{
	CheckNullResult(Guard, false);

	return true;
}
