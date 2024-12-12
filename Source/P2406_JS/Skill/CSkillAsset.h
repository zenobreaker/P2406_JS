#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Skill/CSkillStructures.h"
#include "Skill/CActiveSkill.h"
#include "CSkillAsset.generated.h"


/// <summary>
/// ��ų�� ����ϴ� ������ ���Ǹ� ���� �ִ� ������ ���� �ΰ� 
/// ��Ƽ�� ��ų�� �װ� ��� �޾Ƽ� ó���Ұų�??? 
/// </summary>

UCLASS()
class P2406_JS_API UCSkillAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Skill")
	FSkillInfo SkillInfo;

	// ��Ÿ�ӿ� ������ ActiveSkill�� Ŭ���� ����
	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<UCActiveSkill> SkillClass;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TArray<FSkillActionData> DoActionDatas;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TArray<FSkillHitData> HitDatas;
	
public:
	UCSkillAsset(); 

	void BeginPlay(class ACharacter* InOwner, class UCActiveSkill** OutActiveSkill);
};
