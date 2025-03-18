#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Notifies/CAnimNotify_Structures.h"
#include "CAnimNotify_BossPhaseTransition.generated.h"


UCLASS()
class P2406_JS_API UCAnimNotify_BossPhaseTransition : public UAnimNotify
{
	GENERATED_BODY()


private:
	UPROPERTY(EditAnywhere, Category = "Type")
	EAnimNotify_Flow Type = EAnimNotify_Flow::Max;


public:
	UCAnimNotify_BossPhaseTransition();

private:
	FString GetNotifyName_Implementation() const override;
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
