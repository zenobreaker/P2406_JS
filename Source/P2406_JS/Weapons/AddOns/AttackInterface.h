#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AttackInterface.generated.h"

UINTERFACE(MinimalAPI)
class UAttackInterface : public UInterface
{
	GENERATED_BODY()
};

class P2406_JS_API IAttackInterface
{
	GENERATED_BODY()

public:

    // 기본적으로 false 반환
    virtual bool CanAttack() const { return false; }
    // 이 인터페이스를 상속받은 자가 공격에 관한 처리 중인지
    virtual bool IsEnable() const { return false; }
    //// 공격 주체 (어느 캐릭터가 공격했는가?)
    virtual AActor* GetDamageSource() const = 0;

    //// 공격력이 얼마인지?
    //virtual float GetAttackDamage() const = 0;

    //// 트레이스에 감지되었을 때 처리할 함수 (옵션)
    //virtual void OnHitDetected(AActor* HitActor) {}

    //// 투사체라면 충돌 시 제거할 것인지?
    //virtual void DestroyOnImpact() {}
};




//// 피해를 줄 수 있는 오브젝트 (칼질, 총알 등)
//UINTERFACE(MinimalAPI)
//class UDamageDealerInterface : public UInterface
//{
//    GENERATED_BODY()
//};
//
//class IDamageDealerInterface
//{
//    GENERATED_BODY()
//
//public:
//    virtual float GetAttackDamage() const = 0;
//    virtual AActor* GetDamageSource() const = 0;
//};