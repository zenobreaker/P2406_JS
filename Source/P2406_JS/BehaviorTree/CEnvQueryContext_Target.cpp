#include "BehaviorTree/CEnvQueryContext_Target.h"
#include "Global.h"
#include "Characters/CEnemy_AI.h"
#include "Components/CAIBehaviorComponent.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "EnvironmentQuery/EnvQueryTypes.h"

void UCEnvQueryContext_Target::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	// Owner�� QueryContext�� ���� ��Ű�� ������ (Enemy)
	// ����Ʈ �������̹Ƿ� Get�ϸ� �ȴ�.
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(QueryInstance.Owner.Get());
	UCAIBehaviorComponent* behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);

	UEnvQueryItemType_Actor::SetContextHelper(ContextData, behavior->GetTarget());
}