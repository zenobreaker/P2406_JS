#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_SkillCollision.generated.h"


UCLASS()
class P2406_JS_API UCAnimNotify_SkillCollision : public UAnimNotify
{
	GENERATED_BODY()


private:
	FString GetNotifyName_Implementation() const override;
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
};