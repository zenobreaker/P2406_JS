#include "SkillToolbarButton.h"
#include "LevelEditor.h"

#include "SkillAssetEditor.h"
#include "SkillStyle.h"
#include "Skill//CSkillAsset.h"

FSkillToolbarButton::FSkillToolbarButton()
	: TCommands("ToolBar_Button", FText(), NAME_None, FAppStyle::GetAppStyleSetName())
{
	Command = MakeShareable(new FUICommandList());
}

FSkillToolbarButton::~FSkillToolbarButton()
{
	if (Command.IsValid())
		Command.Reset();

	if (Extender.IsValid())
		Extender.Reset();
}

void FSkillToolbarButton::StartUp()
{
	FSkillToolbarButton::RegisterCommands();

	Extender = MakeShareable(new FExtender());

	FToolBarExtensionDelegate toolBarDelegate;
	toolBarDelegate.BindRaw(this, &FSkillToolbarButton::OnAdd_Buttons);
	Extender->AddToolBarExtension("Play", EExtensionHook::After, Command, toolBarDelegate);

	FLevelEditorModule& levelEditor = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	levelEditor.GetToolBarExtensibilityManager()->AddExtender(Extender);
}

void FSkillToolbarButton::RegisterCommands()
{
#define LOCTEXT_NAMESPACE ""
	UI_COMMAND(SkillInfo, "SkillInfo", "", EUserInterfaceActionType::Button, FInputChord());
#undef LOCTEXT_NAMESPACE

	FExecuteAction save = FExecuteAction::CreateRaw(this, &FSkillToolbarButton::OnClicked_Skill);
	Command->MapAction(SkillInfo, save);
}

void FSkillToolbarButton::OnAdd_Buttons(FToolBarBuilder& InBuilder)
{
	InBuilder.AddSeparator();
	InBuilder.AddToolBarButton(SkillInfo, NAME_None, FText(), FText::FromString("Managed Weapon Assets"), FSkillStyle::Get()->SkillIcon);
}

void FSkillToolbarButton::OnClicked_Skill()
{
	//GLog->Log("OnClick _ Skill");
	FSkillAssetEditor::OpenWindow();
}
