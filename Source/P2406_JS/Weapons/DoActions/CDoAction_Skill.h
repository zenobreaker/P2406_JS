#pragma once

#include "CoreMinimal.h"
#include "Weapons/CDoAction.h"
#include "CDoAction_Skill.generated.h"


UCLASS()
class P2406_JS_API UCDoAction_Skill : public UCDoAction
{
	GENERATED_BODY()
	
public:
	void DoAction() override; 
	void Begin_DoAction() override; 
	void End_DoAction() override; 
	void OnAttachmentEndCollision() override;

public:
	void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther) override;
	void OnAttachmentEndOverlap(class ACharacter* InAttacker, class ACharacter* InOther) override;

private:

};
