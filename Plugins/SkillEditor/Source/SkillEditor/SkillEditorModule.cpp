#include "SkillEditorModule.h"
#include "AssetToolsModule.h"
#include "IAssetTools.h"
#include "LevelEditor.h"




#define LOCTEXT_NAMESPACE "FSkillEditorModule"

void FSkillEditorModule::StartupModule()
{
	IAssetTools& assetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	EAssetTypeCategories::Type categoryType = assetTools.RegisterAdvancedAssetCategory("SkillAsset", FText::FromString("Skill"));

	/*ContextMenu = MakeShareable(new FWeaponContextMenu(categoryType));
	assetTools.RegisterAssetTypeActions(ContextMenu.ToSharedRef());*/

}

void FSkillEditorModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSkillEditorModule, SkillEditor)