#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CAnimNotifyState_Equip.generated.h"


UCLASS()
class P2406_JS_API UCAnimNotifyState_Equip : public UAnimNotifyState
{
	GENERATED_BODY()
	
private:
	FString GetNotifyName_Implementation() const override;

	void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

};
