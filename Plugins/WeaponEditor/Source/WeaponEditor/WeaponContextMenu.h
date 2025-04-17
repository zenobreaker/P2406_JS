#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"
#include "AssetTypeCategories.h"


class WEAPONEDITOR_API FWeaponContextMenu
	: public FAssetTypeActions_Base
{
public:
	FWeaponContextMenu(EAssetTypeCategories::Type InType); 

public:
	FText GetName() const override;
	UClass* GetSupportedClass() const override;
	FColor GetTypeColor() const override;
	uint32 GetCategories() override;

	void OpenAssetEditor(const TArray<UObject*>& InObjects,
		const EAssetTypeActivationOpenedMethod OpenedMethod,
		TSharedPtr<IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;


private:
	EAssetTypeCategories::Type Category; 
	
};
