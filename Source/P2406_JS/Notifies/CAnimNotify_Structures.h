#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_Structures.generated.h"


UENUM()
enum class EAnimNotify_Flow : uint8
{
	Begin, Middle, End, Max,
};

UCLASS()
class P2406_JS_API UCAnimNotify_Structures : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	static FColor GetNotifyColor(EAnimNotify_Flow InValue);
	static FString GetNotifyName(FString InName, EAnimNotify_Flow InValue);
};
