#include "BehaviorTree/BTTask/CBTTaskNode_Combo.h"
#include "Global.h"

#include "Characters/CEnemy_AI.h"
#include "Characters/CAIController.h"

#include "Components/CStateComponent.h"
#include "Components/CWeaponComponent.h"
#include "Components/CAIBehaviorComponent.h"

#include "Weapons/DoActions/CDoAction_Combo.h"

UCBTTaskNode_Combo::UCBTTaskNode_Combo()
{
	NodeName = "Combo";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Combo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(ai);
	CheckNullResult(weapon, EBTNodeResult::Failed);
	CheckTrueResult(weapon->IsUnarmedMode(), EBTNodeResult::Failed);

	CurrentValue = FMath::RandRange(0.0f, (float)ComboDecideValue.Y);

	controller->StopMovement();
	weapon->DoAction();

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Combo::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);


	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(ai);
	UCStateComponent* state = FHelpers::GetComponent<UCStateComponent>(ai);
	if (weapon->GetDoAction() == nullptr)
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

	if (CurrentValue < ComboDecideValue.X)
	{

		UCAIBehaviorComponent* behavior = FHelpers::GetComponent<UCAIBehaviorComponent>(ai);
		if (!!behavior)
		{
			behavior->SetWaitMode();
		}

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

		return;
	}

	bool bCheck = true;
	UCDoAction_Combo* combo = Cast<UCDoAction_Combo>(weapon->GetDoAction());
	
	combo->EnableCombo();
	
	bCheck &= (state->IsIdleMode());
	bCheck &= (weapon->GetDoAction()->GetInAction() == false);
	
	// 액션 완료면 한 번 더 
	if (bCheck == false)
		weapon->DoAction();
	else
	{
		UCAIBehaviorComponent* behavior = FHelpers::GetComponent<UCAIBehaviorComponent>(ai);
		if (!!behavior)
		{
			behavior->SetWaitMode();
		}

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

EBTNodeResult::Type UCBTTaskNode_Combo::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(ai);

	bool bBgeinAction = weapon->GetDoAction()->GetBeginAction();

	if (bBgeinAction == false)
		weapon->GetDoAction()->Begin_DoAction();

	weapon->GetDoAction()->End_DoAction();

	return EBTNodeResult::Succeeded;
}
