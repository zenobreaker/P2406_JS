#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CSkillComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkillUsed, UCSkillAsset*, InSkillAsset);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class P2406_JS_API UCSkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCSkillComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FOnSkillUsed OnSkillUsed;

	void UseSkill(UCSkillAsset* InSkillAsset);

	void SetSkillList(const TArray<UCSkillAsset>& InSkills);

private:
	TMap<EKeys, class UCSkillAsset> SkillTable;
};
