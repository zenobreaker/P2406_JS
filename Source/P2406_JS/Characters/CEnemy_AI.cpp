#include "Characters/CEnemy_AI.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Components/CHealthPointComponent.h"
#include "Components/CAIBehaviorComponent.h"
#include "Components/WidgetComponent.h"
#include "Widgets/CUserWidget_Enemy.h"
#include "BehaviorTree/BehaviorTree.h"

ACEnemy_AI::ACEnemy_AI()
{
	PrimaryActorTick.bCanEverTick = true;


	FHelpers::CreateComponent<UWidgetComponent>(this, &LabelWidget, "Label", GetMesh());

	FHelpers::CreateActorComponent<UCWeaponComponent>(this, &Weapon, "Weapon");
	FHelpers::CreateActorComponent<UCAIBehaviorComponent>(this, &Behavior, "Behavior");

	TSubclassOf<UCUserWidget_Enemy> labelClass;
	FHelpers::GetClass(&labelClass, "/Script/UMGEditor.WidgetBlueprint'/Game/Widgets/WB_Enemy.WB_Enemy_C'");

	LabelWidget->SetWidgetClass(labelClass);
	LabelWidget->SetRelativeLocation(FVector(0, 0, 270));
	LabelWidget->SetDrawSize(FVector2D(120, 0));
	LabelWidget->SetWidgetSpace(EWidgetSpace::Screen);

	FHelpers::GetAsset<UBehaviorTree>(&BehaviorTree, "/Script/AIModule.BehaviorTree'/Game/Enemies/Melee/BT_Melee.BT_Melee'");
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
	if(State->IsGuardMode())
		bBlocking = CheckBlocking(DamageData);

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

bool ACEnemy_AI::CanGuard() const
{
	return bCanGuard;
}

void ACEnemy_AI::StartGuard()
{
	// 가드 상태를 할 수 없다면 호출해도 의미 없게 
	CheckFalse(bCanGuard);

	bGuarding = true;

	State->SetGuardMode();

	GuardHP = MaxGuardHealth;
	//PlayAnimMontage(GuardMontage, 1.0f);
}

void ACEnemy_AI::StopGuard()
{
	bGuarding = false;

	State->SetIdleMode();
	UCUserWidget_Enemy* enemyLabel = Cast<UCUserWidget_Enemy>(LabelWidget->GetUserWidgetObject());
	enemyLabel->UpdateGuardGaugeVisibility(false);
	//StopAnimMontage(GuardMontage);
}

bool ACEnemy_AI::CheckBlocking(FDamageData& InDamagedata)
{
	if (GuardHP <= 0.0f)
		return false; 

	FVector attackerLocation = InDamagedata.Attacker->GetActorLocation();

	FVector toAttack = (attackerLocation - GetActorLocation()).GetSafeNormal();
	FVector forward = GetActorForwardVector();

	float dotProduct = FVector::DotProduct(forward, toAttack);

	if(dotProduct > FMath::Cos(FMath::DegreesToRadians(GuardAngle)))
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), GuardSound, GetActorLocation());

		GuardHP += (MaxGuardHealth * 0.1f) * -1.0f;
		UCUserWidget_Enemy* enemyLabel = Cast<UCUserWidget_Enemy>(LabelWidget->GetUserWidgetObject());

		enemyLabel->UpdateGuardGaugeVisibility(true);
		enemyLabel->UpdateGuardGauge(GuardHP, MaxGuardHealth);

		return true; 
	}

	return false; 
}

bool ACEnemy_AI::GetGuarding() const
{
	return bGuarding;
}
