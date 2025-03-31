#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HAL/CriticalSection.h" // �߰�!
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
	// �����ڵ鿡�� ��ū�� �ο� �Ѵ�. 
	void SetTokenAttacker(class ACEnemy_AI* InAttacker);

	// Ÿ�ٿ��� ��ū���� �ο��Ѵ�. 
	void SetTokenTarget(class AActor* InTarget); 

public:
	bool IsContainFromAttackers(class AActor* InTarget, class ACEnemy_AI* InAttacker) const;

	TArray<class ACEnemy_AI*> GetAttackers(AActor* InTarget) const;

	// ����� ������ ���̶�� �˸��ϰ� �������� ���θ� ��ȯ
	bool IsAttackableToTarget(class AActor* InTarget, class ACEnemy_AI* InAttacker);

private:
	TMap<class AActor*, TArray<class ACEnemy_AI*>> TargetToAttackers; // Ÿ�ٺ� ������ ����Ʈ
	FCriticalSection Mutex; // ��Ƽ������ ��ȣ
	// Ư���� �׷� ����
	TMap<int32, TArray<class ACEnemy_AI*>> GroupAITable;


public:
	TMap<AActor*, int32> TargetToTokenCount; // �� Ÿ�ٿ� ���� ���� �Ҵ�� ��ū ��
	const int32 MaxAttackersPerTarget = 1;	// Ÿ�ٿ� ���� ���� �ִ� ���� ���� �� ��	
	static int32 MaxTokenValue;
};
