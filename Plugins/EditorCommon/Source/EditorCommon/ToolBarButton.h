#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"

class EDITORCOMMON_API FToolBarButton
	: public TCommands<FToolBarButton>
{
public:
	FToolBarButton();
	~FToolBarButton();

	void StartUp()
	{
	}

public:
	void RegisterCommands() override;

public:
	TSharedPtr<FUICommandList> Command;
	TSharedPtr<FUICommandInfo> WeaponInfo;

private:
	TSharedPtr<FExtender> Extender;

private:
	void OnAdd_Buttons(class FToolBarBuilder& InBuilder);
	void OnClicked_Weapon();
};
