#include "Widgets/CUserWidget_BuffSelectHUD.h"
#include "Global.h"
#include "Components/UniformGridSlot.h"
#include "Widgets/CUserWidget_BuffSelctionSlot.h"


void UCUserWidget_BuffSelectHUD::NativeConstruct()
{
	Super::NativeConstruct();
}

void UCUserWidget_BuffSelectHUD::AddChild(UWidget* InSlot)
{
	GridPanel = Cast<UUniformGridPanel>(GetWidgetFromName(L"GridPanel"));
	CheckNull(GridPanel);

	UUniformGridSlot* slot = GridPanel->AddChildToUniformGrid(InSlot);
	CheckNull(slot);

	slot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
	slot->SetColumn(ColumnCount++);
}

void UCUserWidget_BuffSelectHUD::SetupBuffs(const TArray<struct FStatBuff>& InBuffs)
{
	CheckNull(BuffSlotClass);

	for (const FStatBuff& buff : InBuffs)
	{
		UCUserWidget_BuffSelctionSlot* buffslot = CreateWidget<UCUserWidget_BuffSelctionSlot>(this, BuffSlotClass);
		if (buffslot == nullptr)
			continue;
		buffslot->SetBuff(buff);
		AddChild(buffslot);
	}
}


void UCUserWidget_BuffSelectHUD::ShowBuffHUD()
{
	this->SetVisibility(ESlateVisibility::Visible);

	if (OnShow_UI_BuffList.IsBound())
		OnShow_UI_BuffList.Broadcast();
}

void UCUserWidget_BuffSelectHUD::HideBuffHUD()
{
	if (SelectedBuffSlot == nullptr)
		return; 

	this->SetVisibility(ESlateVisibility::Hidden);

	if (OnHide_Ui_BuffList.IsBound())
		OnHide_Ui_BuffList.Broadcast();
}

