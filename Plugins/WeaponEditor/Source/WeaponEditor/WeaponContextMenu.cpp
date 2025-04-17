#include "WeaponContextMenu.h"
#include "WeaponAssetEditor.h"

#include "Weapons/CWeaponAsset.h"

FWeaponContextMenu::FWeaponContextMenu(EAssetTypeCategories::Type InType)
{
	Category = InType; 
}

FText FWeaponContextMenu::GetName() const
{
	return FText::FromString("WeaponAsset");
}

UClass* FWeaponContextMenu::GetSupportedClass() const
{
	return UCWeaponAsset::StaticClass();
}

FColor FWeaponContextMenu::GetTypeColor() const
{
	return FColor::Green;
}

uint32 FWeaponContextMenu::GetCategories()
{
	return Category;
}

void FWeaponContextMenu::OpenAssetEditor(const TArray<UObject*>& InObjects, const EAssetTypeActivationOpenedMethod OpenedMethod, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	if (InObjects.Num() < 1)
		return;

	FWeaponAssetEditor::OpenWindow(InObjects[0]->GetName());
}
