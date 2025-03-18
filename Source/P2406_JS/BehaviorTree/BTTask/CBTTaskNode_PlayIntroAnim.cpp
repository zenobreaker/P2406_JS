#include "BehaviorTree/BTTask/CBTTaskNode_PlayIntroAnim.h"
#include "Global.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"

#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Components/CAIBehaviorComponent.h"


UCBTTaskNode_PlayIntroAnim::UCBTTaskNode_PlayIntroAnim()
{
	NodeName = "IntroAnim";
}

EBTNodeResult::Type UCBTTaskNode_PlayIntroAnim::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller =  Cast<ACAIController>(OwnerComp.GetAIOwner());
	if (controller == nullptr) return EBTNodeResult::Failed;

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	if (ai == nullptr) return EBTNodeResult::Failed;

	UCAIBehaviorComponent* behavior = FHelpers::GetComponent<UCAIBehaviorComponent>(ai);
	if(behavior == nullptr) return EBTNodeResult::Failed;

	UBlackboardComponent* blackboard = behavior->GetBlackboard();
	if(blackboard == nullptr) return EBTNodeResult::Failed;
	
	// 키 값 가져오기 
	bool bCompleteIntro = blackboard->GetValueAsBool(BoardKey.SelectedKeyName);
	if(bCompleteIntro == true) return EBTNodeResult::Failed;

	// 몽타주 재생 
	if(IntroMontage == nullptr) return EBTNodeResult::Failed;
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

void UCBTTaskNode_PlayIntroAnim::SetBlackboardValue(UBlackboardComponent* InBlackboard)
{
	CheckNull(InBlackboard); 

	FBlackboard::FKey key = InBlackboard->GetKeyID(BoardKey.SelectedKeyName);
	TSubclassOf<UBlackboardKeyType> keyTypeClass =  InBlackboard->GetKeyType(key); 
	
	if (!!keyTypeClass && keyTypeClass->IsChildOf(UBlackboardKeyType_Bool::StaticClass()))
	{
		InBlackboard->SetValueAsBool(BoardKey.SelectedKeyName, true);
	}
}

