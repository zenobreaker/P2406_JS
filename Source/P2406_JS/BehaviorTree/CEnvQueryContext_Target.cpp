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
	CheckNull(ai);
	UCAIBehaviorComponent* behavior = FHelpers::GetComponent<UCAIBehaviorComponent>(ai);
	CheckNull(behavior);

	auto target = behavior->GetTarget();
	CheckNull(target);

	UEnvQueryItemType_Actor::SetContextHelper(ContextData, target);
}