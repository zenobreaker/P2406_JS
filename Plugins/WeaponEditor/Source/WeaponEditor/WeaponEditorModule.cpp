#include "WeaponEditorModule.h"
#include "AssetToolsModule.h"
#include "IAssetTools.h"

#include "WeaponStyle.h"
#include "WeaponContextMenu.h"
#include "WeaponToolBarButton.h"

#define LOCTEXT_NAMESPACE "FWeaponEditorModule"

void FWeaponEditorModule::StartupModule()
{
	IAssetTools& assetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	EAssetTypeCategories::Type categoryType = assetTools.RegisterAdvancedAssetCategory("WeaponAsset", FText::FromString("Weapon"));

	ContextMenu = MakeShareable(new FWeaponContextMenu(categoryType));
	assetTools.RegisterAssetTypeActions(ContextMenu.ToSharedRef());

	FWeaponStyle::Regist();

	ToolBarButton = MakeShareable(new FWeaponToolBarButton());
	ToolBarButton->StartUp(); 
}

void FWeaponEditorModule::ShutdownModule()
{

	if (ToolBarButton.IsValid())
		ToolBarButton.Reset();

	FWeaponStyle::Unregist();

	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& assetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
		assetTools.UnregisterAssetTypeActions(ContextMenu.ToSharedRef());
	}

	if (ContextMenu.IsValid())
		ContextMenu.Reset(); 

}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FWeaponEditorModule, WeaponEditor)