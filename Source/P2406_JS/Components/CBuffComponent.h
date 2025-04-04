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
	void ApplyBuff(const FStatBuff& NewBuff);  // ���� �߰�
	void RemoveBuff(const FStatBuff& InStatBuff); // Ư�� ���� ����
	void RemoveBuff(const ECharStatType StatType, float Value); // Ư�� ���� ����
	void RemoveExpiredBuffs(); // ����� ���� ����
	void ClearAllBuffs(); // ��� ���� ����

public:
	UFUNCTION()
	void OnBuffExpired(ECharStatType InStatType, float Value);

private:
	UPROPERTY()
	TArray<FStatBuff> ActiveBuffs; // ���� ����� ���� ����Ʈ

private:
	class ACharacter* OwnerCharacter;
};
