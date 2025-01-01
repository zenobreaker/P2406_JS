#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IGuardable.generated.h"

UINTERFACE(MinimalAPI)
class UIGuardable : public UInterface
{
	GENERATED_BODY()
};

class P2406_JS_API IIGuardable
{
	GENERATED_BODY()

public:
	virtual bool HasGuard() const = 0;
	static bool Execute_HasGuard(const UObject* Object)
	{
		const IIGuardable* Guardable = Cast<IIGuardable>(Object);
		if (Guardable)
		{
			return Guardable->HasGuard();
		}
		return false;  // �⺻������ false ��ȯ
	}

	// ���� �������� üũ�ϴ� �޼���
	virtual bool CanGuard() const = 0;

	virtual bool GetGuarding() const = 0; 

	// ���带 �����ϴ� �޼���
	virtual void StartGuard() = 0;

	// ���带 �����ϴ� �޼���
	virtual void StopGuard() = 0;

};
