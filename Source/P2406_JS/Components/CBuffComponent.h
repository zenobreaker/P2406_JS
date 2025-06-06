#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Buffs/CBuffStructures.h"
#include "CBuffComponent.generated.h"



UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class P2406_JS_API UCBuffComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCBuffComponent();

protected:
	virtual void BeginPlay() override;

public:
	void ApplyBuff(const FStatBuff& NewBuff);  // 버프 추가
	void RemoveBuff(const FStatBuff& InStatBuff); // 특정 버프 제거
	void RemoveBuff(const ECharStatType StatType, float Value); // 특정 버프 제거
	void RemoveExpiredBuffs(); // 만료된 버프 정리
	void ClearAllBuffs(); // 모든 버프 삭제

public:
	UFUNCTION()
	void OnBuffExpired(ECharStatType InStatType, float Value);

private:
	UPROPERTY()
	TArray<FStatBuff> ActiveBuffs; // 현재 적용된 버프 리스트

private:
	class ACharacter* OwnerCharacter;
};
