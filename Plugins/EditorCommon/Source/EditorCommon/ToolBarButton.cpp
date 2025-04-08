#include "ToolBarButton.h"
#include "LevelEditor.h"



FToolBarButton::FToolBarButton()
	: TCommands("ToolBar_Button", FText(), NAME_None, FAppStyle::GetAppStyleSetName())
{
	Command = MakeShareable(new FUICommandList());
}

FToolBarButton::~FToolBarButton()
{
}

void FToolBarButton::RegisterCommands()
{
}

void FToolBarButton::OnAdd_Buttons(FToolBarBuilder& InBuilder)
{
}

void FToolBarButton::OnClicked_Weapon()
{
}

