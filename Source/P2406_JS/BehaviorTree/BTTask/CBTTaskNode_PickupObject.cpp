#include "BehaviorTree/BTTask/CBTTaskNode_PickupObject.h"
#include "Global.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"

#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Components/CAIBehaviorComponent.h"


UCBTTaskNode_PickupObject::UCBTTaskNode_PickupObject()
{
	NodeName = "PickupObject";
}

EBTNodeResult::Type UCBTTaskNode_PickupObject::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetAIOwner());
	if (controller == nullptr) return EBTNodeResult::Failed;

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	if (ai == nullptr) return EBTNodeResult::Failed;

	UCAIBehaviorComponent* behavior = FHelpers::GetComponent<UCAIBehaviorComponent>(ai);
	if (behavior == nullptr) return EBTNodeResult::Failed;

	UBlackboardComponent* blackboard = behavior->GetBlackboard();
	if (blackboard == nullptr) return EBTNodeResult::Failed;

	// 키 값 가져오기 
	bool bIsOpening = blackboard->GetValueAsBool(BoardKey.SelectedKeyName);
	if (bIsOpening == false) return EBTNodeResult::Failed;

	// 몽타주 재생 
	if (IntroMontage == nullptr) return EBTNodeResult::Failed;
	ai->PlayAnimMontage(IntroMontage, PlayRate);


	// 애니메이션 타이머 실행 
	float animDuration = IntroMontage->GetPlayLength();
	ai->GetWorldTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([ai, blackboard, &OwnerComp, this]()
		{
			SetBlackboardValue(blackboard);

			if (ai != nullptr)
				ai->GetWorldTimerManager().ClearTimer(Handle);

			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}), animDuration, false);

	return EBTNodeResult::InProgress;
}

//TODO: 나중에 공통 로직으로 리팩토링하기
void UCBTTaskNode_PickupObject::SetBlackboardValue(UBlackboardComponent* InBlackboard)
{
	CheckNull(InBlackboard);

	FBlackboard::FKey key = InBlackboard->GetKeyID(BoardKey.SelectedKeyName);
	TSubclassOf<UBlackboardKeyType> keyTypeClass = InBlackboard->GetKeyType(key);

	if (!!keyTypeClass && keyTypeClass->IsChildOf(UBlackboardKeyType_Bool::StaticClass()))
	{
		InBlackboard->SetValueAsBool(BoardKey.SelectedKeyName, false);
	}
}

