#include "Components/AddOns/CPatternDecider.h"
#include "Global.h"
#include "GameFramework/Character.h"

UCPatternDecider::UCPatternDecider()
{
	// 패턴 아이디 초기화 
	MaintainPatternID = -1; 
}

void UCPatternDecider::BeginPlay()
{
	// 매 레벨마다 실행 시에 유효하게 
	MaintainPatternID = -1;
}

EPatternDecision UCPatternDecider::EvaluatePattern() const
{
	if (FMath::RandRange(0, 1) <= KeepPatternRatio)
		return EPatternDecision::KeepPattern;

	//TODO :  여러 조건에 한하여 패턴 변경이나 취소 
	return EPatternDecision::ChangePattern;
}


