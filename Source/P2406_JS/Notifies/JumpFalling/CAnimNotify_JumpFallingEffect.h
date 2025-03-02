#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_JumpFallingEffect.generated.h"


UCLASS()
class P2406_JS_API UCAnimNotify_JumpFallingEffect : public UAnimNotify
{
	GENERATED_BODY()

public:
	UCAnimNotify_JumpFallingEffect();

private:
	FString GetNotifyName_Implementation() const override;
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

};
