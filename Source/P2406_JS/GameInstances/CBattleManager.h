#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HAL/CriticalSection.h" // 추가!
#include "CBattleManager.generated.h"


UCLASS(Blueprintable)
class P2406_JS_API UCBattleManager : public UObject
{
	GENERATED_BODY()

public:
	UCBattleManager();


public:
	void RegistGroup(int32 InGroupID, class ACEnemy_AI* InMember);

	void UnregistGroup(int32 InGroupID, class ACEnemy_AI* InMember);

	class ACharacter* GetBattleAttackerOfTarget(int32 InGroupID, class ACEnemy_AI* InCaller);

	void RequestBattleParticipation(int32 InGroupID, class ACEnemy_AI* InInitiator, class ACharacter* InTarget);


public:
	void RegistBattle(class AActor* InTarget, class ACEnemy_AI* InAttacker);
	
	void UnregistBattle(class AActor* InTarget, class ACEnemy_AI* InAttacker);

public:
	UFUNCTION()
	void UnregistAttacker(class ACharacter* InAttacker);

	void UnregistAttacker(class AActor* InaTarget, class ACEnemy_AI* InAttacker);

	UFUNCTION()
	void UnregistTarget(class ACharacter* InTarget);


private:
	// 공격자들에게 토큰을 부여 한다. 
	void SetTokenAttacker(class ACEnemy_AI* InAttacker);

	// 타겟에게 토큰값을 부여한다. 
	void SetTokenTarget(class AActor* InTarget); 

public:
	bool IsContainFromAttackers(class AActor* InTarget, class ACEnemy_AI* InAttacker) const;

	TArray<class ACEnemy_AI*> GetAttackers(AActor* InTarget) const;

	// 대상을 공격할 것이라고 알림하고 가능한지 여부를 반환
	bool IsAttackableToTarget(class AActor* InTarget, class ACEnemy_AI* InAttacker);

private:
	TMap<class AActor*, TArray<class ACEnemy_AI*>> TargetToAttackers; // 타겟별 공격자 리스트
	FCriticalSection Mutex; // 멀티스레드 보호
	// 특정한 그룹 모음
	TMap<int32, TArray<class ACEnemy_AI*>> GroupAITable;


public:
	TMap<AActor*, int32> TargetToTokenCount; // 각 타겟에 대해 현재 할당된 토큰 수
	const int32 MaxAttackersPerTarget = 1;	// 타겟에 대해 현재 최대 공격 가능 적 수	
	static int32 MaxTokenValue;
};
