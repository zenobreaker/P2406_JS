
#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FWeaponEditorModule : public IModuleInterface
{
public:

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private: 
	TSharedPtr<class FWeaponContextMenu> ContextMenu;
	TSharedPtr<class FWeaponToolBarButton> ToolBarButton;
};
