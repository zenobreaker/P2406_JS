#include "Components/AddOns/CPatternDecider.h"
#include "Global.h"
#include "GameFramework/Character.h"

UCPatternDecider::UCPatternDecider()
{
	// ���� ���̵� �ʱ�ȭ 
	MaintainPatternID = -1; 
}

void UCPatternDecider::BeginPlay()
{
	// �� �������� ���� �ÿ� ��ȿ�ϰ� 
	MaintainPatternID = -1;
}

EPatternDecision UCPatternDecider::EvaluatePattern() const
{
	if (FMath::RandRange(0, 1) <= KeepPatternRatio)
		return EPatternDecision::KeepPattern;

	//TODO :  ���� ���ǿ� ���Ͽ� ���� �����̳� ��� 
	return EPatternDecision::ChangePattern;
}


