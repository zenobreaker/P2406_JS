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

    // �⺻������ false ��ȯ
    virtual bool CanAttack() const { return false; }
    // �� �������̽��� ��ӹ��� �ڰ� ���ݿ� ���� ó�� ������
    virtual bool IsEnable() const { return false; }
    //// ���� ��ü (��� ĳ���Ͱ� �����ߴ°�?)
    virtual AActor* GetDamageSource() const = 0;

    //// ���ݷ��� ������?
    //virtual float GetAttackDamage() const = 0;

    //// Ʈ���̽��� �����Ǿ��� �� ó���� �Լ� (�ɼ�)
    //virtual void OnHitDetected(AActor* HitActor) {}

    //// ����ü��� �浹 �� ������ ������?
    //virtual void DestroyOnImpact() {}
};




//// ���ظ� �� �� �ִ� ������Ʈ (Į��, �Ѿ� ��)
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