#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_Structures.h"
#include "CAnimNotify_Move.generated.h"


UCLASS()
class P2406_JS_API UCAnimNotify_Move : public UAnimNotify
{
	GENERATED_BODY()
	
	
private:
	UPROPERTY(EditAnywhere, Category = "Type")
	EAnimNotify_Flow Type = EAnimNotify_Flow::Max;

	UPROPERTY(EditAnywhere, Category = "Movement")
	TEnumAsByte<EMovementMode> MovementMode = EMovementMode::MOVE_None;

public:
	UCAnimNotify_Move(); 

private:
	FString GetNotifyName_Implementation() const override;
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
