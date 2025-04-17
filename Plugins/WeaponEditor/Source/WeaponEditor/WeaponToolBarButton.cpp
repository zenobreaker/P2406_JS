#include "WeaponToolBarButton.h"
#include "LevelEditor.h"

#include "WeaponAssetEditor.h"
#include "WeaponStyle.h"
#include "Weapons/CWeaponAsset.h"

FWeaponToolBarButton::FWeaponToolBarButton()
	:TCommands("ToolBar_Button", FText(), NAME_None, FAppStyle::GetAppStyleSetName())
{
	Command = MakeShareable(new FUICommandList());
}

FWeaponToolBarButton::~FWeaponToolBarButton()
{
	if (Command.IsValid())
		Command.Reset();

	if (Extender.IsValid())
		Extender.Reset(); 
}

void FWeaponToolBarButton::StartUp()
{
	FWeaponToolBarButton::RegisterCommands(); 

	Extender = MakeShareable(new FExtender());

	FToolBarExtensionDelegate toolBarDelegate;
	toolBarDelegate.BindRaw(this, &FWeaponToolBarButton::OnAdd_Buttons); 
	Extender->AddToolBarExtension("Play", EExtensionHook::After, Command, toolBarDelegate); 

	FLevelEditorModule& levelEditor = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	levelEditor.GetToolBarExtensibilityManager()->AddExtender(Extender);
}

void FWeaponToolBarButton::RegisterCommands()
{
#define LOCTEXT_NAMESPACE ""
	UI_COMMAND(WeaponInfo, "WeaponInfo", "", EUserInterfaceActionType::Button, FInputChord());
#undef LOCTEXT_NAMESPACE

	FExecuteAction save = FExecuteAction::CreateRaw(this, &FWeaponToolBarButton::OnClicked_Weapon);
	Command->MapAction(WeaponInfo, save);
}


void FWeaponToolBarButton::OnAdd_Buttons(FToolBarBuilder& InBuilder)
{
	InBuilder.AddSeparator(); 
	InBuilder.AddToolBarButton(WeaponInfo, NAME_None, FText(), FText::FromString("Managed Weapon Assets"), FWeaponStyle::Get()->WeaponIcon);
}

void FWeaponToolBarButton::OnClicked_Weapon()
{
	FWeaponAssetEditor::OpenWindow();
}
