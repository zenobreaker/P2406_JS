#pragma once

#include "CoreMinimal.h"
#include "CSkillStructures.h"
//#include "SkillExecuter.generated.h"

class P2406_JS_API SkillExecuter
{

public:
	// 스킬 실행
	static void ExecuteSkillPhase(ACharacter* InCharacter, FSkillPhaseData& PhaseData)
	{
		// 1 액션 실행
		Phase_DoAction(InCharacter, PhaseData.ActionData);

		// 2 이펙트 재생
		if (PhaseData.Effect)
			Phase_PlayEffect(InCharacter, PhaseData.Effect, PhaseData.EffectLocation, PhaseData.EffectScale);

		// 3 사운드 재생
		if (PhaseData.Sound)
			Phase_PlaySound(InCharacter, PhaseData.Sound);

		// 4 카메라 흔들림
		if (PhaseData.CameraShake)
			Phase_PlayCameraShake(InCharacter, PhaseData.CameraShake);

		// 5 스킬 엔티티 스폰
		Phase_SpawnSkillEntity(InCharacter, PhaseData.SkillEntityData);
	}

private:
	// 액션 실행
	static void Phase_DoAction(ACharacter* InCharacter, FDoActionData& ActionData);

	// 이펙트 재생
	static void Phase_PlayEffect(ACharacter* InCharacter, UFXSystemAsset* Effect, FVector Location, FVector Scale);

	// 사운드 재생
	static void Phase_PlaySound(ACharacter* InCharacter, USoundWave* Sound);

	// 카메라 흔들림 적용
	static void Phase_PlayCameraShake(ACharacter* InCharacter, TSubclassOf<UCameraShakeBase> CameraShake);

	//  스킬 엔티티 스폰
	static void Phase_SpawnSkillEntity(ACharacter* InCharacter, FSkillEntityData& SkillEntityData);
};