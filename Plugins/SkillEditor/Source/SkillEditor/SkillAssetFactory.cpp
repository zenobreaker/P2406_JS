#include "SkillAssetFactory.h"
#include "Skill/CSkillAsset.h"

USkillAssetFactory::USkillAssetFactory()
{
	bCreateNew = true; 

	SupportedClass = UCSkillAsset::StaticClass(); 
}

UObject* USkillAssetFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	return NewObject<UCSkillAsset>(InParent, InClass, InName, Flags);
}
