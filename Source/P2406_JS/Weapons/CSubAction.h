#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CSubAction.generated.h"


UCLASS()
class P2406_JS_API UCSubAction : public UObject
{
	GENERATED_BODY()

public:
	FORCEINLINE bool GetInAction() { return bInAction; }
	
public:
	UCSubAction();

public:
	virtual void BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment, class UCDoAction* InDoAction);
	virtual void Tick(float InDeltaTime) {}

public:
	virtual void Pressed();
	virtual void Released();

protected:
	bool bInAction;

	class ACharacter* Owner; 
	class ACAttachment* Attachment;
	class UCDoAction* DoAction;

	class UCStateComponent* State;
	class UCMovementComponent* Movement;
};
