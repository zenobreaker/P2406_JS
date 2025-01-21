#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CBattleManager.generated.h"

UCLASS()
class P2406_JS_API UCBattleManager : public UObject
{
	GENERATED_BODY()
	

public:
	UCBattleManager();


public:
	void RegisterGroup(int32 InGroupID, class ACEnemy_AI* InTarget);

	void UnregisterGroup(int32 InGroupID, class ACEnemy_AI* InTarget);

	class ACharacter* GetBattleAttackerOfTarget(int32 InGroupID, class ACEnemy_AI* InCaller);

	void RequestBattleParticipation(int32 InGroupID, class ACEnemy_AI* InInitiator, class ACharacter* InTarget);

public:
	UFUNCTION()
	void RegisterAttacker(class AActor* InTarget, class ACEnemy_AI* InAttacker);
	
	UFUNCTION()
	void UnregisterAttacker(class AActor* InTarget, class ACEnemy_AI* InAttacker);

	bool IsBeingAttacked(AActor* InTarget) const;

	TArray<class ACEnemy_AI*> GetAttackers(AActor* InTarget) const;

	// 대상을 공격할 것이라고 알림하고 가능한지 여부를 반환
	bool IsAttackableToTarget(class AActor* InTarget, class ACEnemy_AI* InAttacker);

private:
	TMap<class AActor*, TArray<class ACEnemy_AI*>> TargetToAttackers; // 타겟별 공격자 리스트
	FCriticalSection Mutex; // 멀티스레드 보호

	TMap<int32, TArray<class ACEnemy_AI*>> GroupAITable; 

};
