#pragma once

#if WITH_EDITOR
#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "WeaponAssetFactory.generated.h"


UCLASS()
class WEAPONEDITOR_API UWeaponAssetFactory : public UFactory
{
	GENERATED_BODY()
	
public:
	UWeaponAssetFactory(); 

	UObject* FactoryCreateNew(UClass* InClass, 
		UObject* InParent, 
		FName InName, 
		EObjectFlags Flags, 
		UObject* Context, 
		FFeedbackContext* Warn, FName CallingContext) override;
};
#endif 