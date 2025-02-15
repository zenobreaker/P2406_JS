#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Skill/CSkillStructures.h"
#include "Skill/CActiveSkill.h"
#include "CSkillAsset.generated.h"


/// <summary>
/// 스킬를 사용하는 형태의 정의를 갖고 있는 정보를 어디다 두고 
/// 액티브 스킬은 그걸 어떻게 받아서 처리할거냐??? 
/// </summary>
UCLASS()
class P2406_JS_API UCSkillAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Skill")
	FSkillInfo SkillInfo;

	// 런타임에 생성할 ActiveSkill의 클래스 참조
	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<UCActiveSkill> SkillClass;

	UPROPERTY(EditAnywhere, Category = "Skill")
	FSkillFlowData FlowData;

	
public:
	UCSkillAsset(); 

	/*template <typename T>
	T* CreateSkillInstance(
		TSubclassOf<T> SkillClass,
		ACharacter* InCharacter,
		const FSkillInfo& InSkillInfo,
		const TArray<FSkillActionData>& InSkillActionDatas,
		const TArray<FSkillHitData>& InSkillHitDatas);*/

	void SkillAsset_BeginPlay(class ACharacter* InOwner, class UCActiveSkill** OutActiveSkill);
};

