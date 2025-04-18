#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FSkillEditorModule : public IModuleInterface
{
public:

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	TSharedPtr<class FSkillContextMenu> ContextMenu;
	TSharedPtr<class FSkillToolbarButton> ToolBarButton;
};
