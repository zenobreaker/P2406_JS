#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Enums/FEnum.h"
#include "CAnimNotify_SkillPhase.generated.h"

UCLASS()
class P2406_JS_API UCAnimNotify_SkillPhase : public UAnimNotify
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = "Type")
	ESkillPhase Type = ESkillPhase::Max;

public:
	UCAnimNotify_SkillPhase();

private:
	FString GetNotifyName_Implementation() const override;
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	FString GetNotifyName(FString InName, ESkillPhase InType) const;
};
