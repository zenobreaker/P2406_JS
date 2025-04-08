#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"


class SKILLEDITOR_API FSkillToolbarButton
	: public TCommands<FSkillToolbarButton>
{
public:
	FSkillToolbarButton();
	~FSkillToolbarButton();

	void StartUp();

public:
	void RegisterCommands() override;

public:
	TSharedPtr<FUICommandList> Command;
	TSharedPtr<FUICommandInfo> SkillInfo;

private:
	TSharedPtr<FExtender> Extender;

private:
	void OnAdd_Buttons(class FToolBarBuilder& InBuilder);
	void OnClicked_Skill();
};
