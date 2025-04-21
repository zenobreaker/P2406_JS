#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Skill/CSkillStructures.h"
#include "Skill/CActiveSkill.h"
#include "CSkillAsset.generated.h"


UCLASS()
class P2406_JS_API UCSkillAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Skill")
	FSkillInfo SkillInfo;

	// 런타임에 생성할 ActiveSkill의 클래스 참조
	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<UCActiveSkill> SkillClass = nullptr;

	UPROPERTY(EditAnywhere, Category = "Skill")
	FSkillFlowData FlowData;

	
public:
	UCSkillAsset(); 

public:
	void SkillAsset_BeginPlay(class ACharacter* InOwner, class UCActiveSkill** OutActiveSkill);
};

