#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_Hammer_Charging.generated.h"

UCLASS()
class P2406_JS_API UCAnimNotify_Hammer_Charging : public UAnimNotify
{
	GENERATED_BODY()
	

private:
	FString GetNotifyName_Implementation() const override;
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
