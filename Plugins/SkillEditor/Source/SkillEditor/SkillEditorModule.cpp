#include "SkillEditorModule.h"
#include "AssetToolsModule.h"
#include "IAssetTools.h"
#include "LevelEditor.h"

#include "SkillContextMenu.h"
#include "SkillStyle.h"
#include "SkillToolbarButton.h"


#define LOCTEXT_NAMESPACE "FSkillEditorModule"

void FSkillEditorModule::StartupModule()
{
	IAssetTools& assetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	EAssetTypeCategories::Type categoryType = assetTools.RegisterAdvancedAssetCategory("SkillAsset", FText::FromString("Skill"));

	ContextMenu = MakeShareable(new FSkillContextMenu(categoryType));
	assetTools.RegisterAssetTypeActions(ContextMenu.ToSharedRef());


	FSkillStyle::Regist(); 

	ToolBarButton = MakeShareable(new FSkillToolbarButton());
	ToolBarButton->StartUp(); 
}

void FSkillEditorModule::ShutdownModule()
{
	if (ToolBarButton.IsValid())
		ToolBarButton.Reset();

	FSkillStyle::Unregist(); 

	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& assetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
		assetTools.UnregisterAssetTypeActions(ContextMenu.ToSharedRef());
	}

	if (ContextMenu.IsValid())
		ContextMenu.Reset();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSkillEditorModule, SkillEditor)