#include "BehaviorTree/BTTask/CBTTaskNode_PlayIntroAnim.h"
#include "Global.h"
#include "BehaviorTree/BlackboardComponent.h"

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
	
	bool bCompleteIntro = blackboard->GetValueAsBool(CompleteIntoKey);
	if(bCompleteIntro == true) return EBTNodeResult::Failed;

	if(IntroMontage == nullptr) return EBTNodeResult::Failed;
	ai->PlayAnimMontage(IntroMontage, PlayRate);
	
	float animDuration = IntroMontage->GetPlayLength();
	
	ai->GetWorldTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([ai, blackboard, &OwnerComp, this]()
		{
			if(blackboard != nullptr )
				blackboard->SetValueAsBool(CompleteIntoKey, true);

			if (ai != nullptr)
				ai->GetWorldTimerManager().ClearTimer(Handle);

			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}), animDuration, false);

	return EBTNodeResult::InProgress;
}

