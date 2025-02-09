#include "Notifies/CAnimNotify_Structures.h"
#include "Global.h"


FColor UCAnimNotify_Structures::GetNotifyColor(EAnimNotify_Flow InValue)
{
#if WITH_EDITORONLY_DATA
	switch (InValue)
	{
		case EAnimNotify_Flow::Begin: return FColor::Green;
		case EAnimNotify_Flow::End: return FColor::Blue;
		case EAnimNotify_Flow::Max: return FColor::Red;
	}
#endif

	return FColor();
}

FString UCAnimNotify_Structures::GetNotifyName(FString InName, EAnimNotify_Flow InValue)
{
	if (InValue != EAnimNotify_Flow::Max)
		return InName + "_" + StaticEnum<EAnimNotify_Flow>()->GetNameStringByValue((int64)InValue);

	return InName;
}
