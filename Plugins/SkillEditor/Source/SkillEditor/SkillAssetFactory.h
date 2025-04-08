#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "SkillAssetFactory.generated.h"

UCLASS()
class SKILLEDITOR_API USkillAssetFactory : public UFactory
{
	GENERATED_BODY()

public:
	USkillAssetFactory(); 

	UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext) override;
};
