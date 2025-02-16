#pragma once

#include "CoreMinimal.h"
#include "CSkillStructures.h"
//#include "SkillExecuter.generated.h"

class P2406_JS_API SkillExecuter
{

public:
	// ��ų ����
	static void ExecuteSkillPhase(ACharacter* InCharacter, FSkillPhaseData& PhaseData)
	{
		// 1 �׼� ����
		Phase_DoAction(InCharacter, PhaseData.ActionData);

		// 2 ����Ʈ ���
		if (PhaseData.Effect)
			Phase_PlayEffect(InCharacter, PhaseData.Effect, PhaseData.EffectLocation, PhaseData.EffectScale);

		// 3 ���� ���
		if (PhaseData.Sound)
			Phase_PlaySound(InCharacter, PhaseData.Sound);

		// 4 ī�޶� ��鸲
		if (PhaseData.CameraShake)
			Phase_PlayCameraShake(InCharacter, PhaseData.CameraShake);

		// 5 ��ų ��ƼƼ ����
		Phase_SpawnSkillEntity(InCharacter, PhaseData.SkillEntityData);
	}

private:
	// �׼� ����
	static void Phase_DoAction(ACharacter* InCharacter, FDoActionData& ActionData);

	// ����Ʈ ���
	static void Phase_PlayEffect(ACharacter* InCharacter, UFXSystemAsset* Effect, FVector Location, FVector Scale);

	// ���� ���
	static void Phase_PlaySound(ACharacter* InCharacter, USoundWave* Sound);

	// ī�޶� ��鸲 ����
	static void Phase_PlayCameraShake(ACharacter* InCharacter, TSubclassOf<UCameraShakeBase> CameraShake);

	//  ��ų ��ƼƼ ����
	static void Phase_SpawnSkillEntity(ACharacter* InCharacter, FSkillEntityData& SkillEntityData);
};