#include "BehaviorTree/Decorator/CBTDecorator_CprFloatLessEqaul.h"
#include "CBTDecorator_CprFloatLessEqaul.h"
#include "BehaviorTree/BlackboardComponent.h"

UCBTDecorator_CprFloatLessEqaul::UCBTDecorator_CprFloatLessEqaul()
{
	NodeName = "FloatLessEqual";
	A.AddFloatFilter(this, NAME_None);
	B.AddFloatFilter(this, NAME_None);
}

bool UCBTDecorator_CprFloatLessEqaul::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const float AValue = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(A.SelectedKeyName);
	const float BValue = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(B.SelectedKeyName);


	return AValue <= BValue;
}

FString UCBTDecorator_CprFloatLessEqaul::GetStaticDescription() const
{
	FString ADesc = A.SelectedKeyName.ToString();
	FString BDesc = B.SelectedKeyName.ToString();

	return FString::Printf(TEXT("Check if %s <= %s"), *ADesc, *BDesc);
}
