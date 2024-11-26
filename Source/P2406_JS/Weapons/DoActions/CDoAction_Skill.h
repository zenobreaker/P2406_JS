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

public:
	void OnAttachmentEndCollision() override;

	void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther) override;
	void OnAttachmentEndOverlap(class ACharacter* InAttacker, class ACharacter* InOther) override;

private:
	int32 Index; // ½ºÅ³º° Phase Index

private:
	TArray<class ACharacter*> Hitted; 
};
