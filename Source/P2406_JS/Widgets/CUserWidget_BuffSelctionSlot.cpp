#include "Widgets/CUserWidget_BuffSelctionSlot.h"
#include "Global.h"
#include "Components/UniformGridPanel.h"

#include "GameInstances/CGameInstance.h"
#include "GameInstances/CBuffUIManager.h"
#include "Widgets/CUserWidget_BuffSelectHUD.h"

void UCUserWidget_BuffSelctionSlot::Clicked()
{
	if (GetBuffListHUD() != nullptr)
	{
		UCUserWidget_BuffSelctionSlot* otherSlot = GetBuffListHUD()->GetSelectedBuffSlot();
		UCUserWidget_BuffSelctionSlot* selcted = nullptr;

		selcted = this; 
		if (otherSlot == this)
			selcted = nullptr;

		for (UWidget* widget : GetBuffListHUD()->GetGridPanel()->GetAllChildren())
		{
			UCUserWidget_BuffSelctionSlot* child = Cast<UCUserWidget_BuffSelctionSlot>(widget);
			if (child == nullptr)
				continue;

			if (selcted == child)
			{
				child->Selected();
				
			}
			else
				child->Unselected();
		}

		GetBuffListHUD()->SelectBuffSlot(selcted);
	}
}

void UCUserWidget_BuffSelctionSlot::Hovered()
{

}

void UCUserWidget_BuffSelctionSlot::Unhovered()
{
}

void UCUserWidget_BuffSelctionSlot::SetBuff_Implementation(const FStatBuff& InBuff)
{
	
}

void UCUserWidget_BuffSelctionSlot::Selected_Implementation()
{
	if (GetBuffListHUD() != nullptr)
		GetBuffListHUD()->SelectBuffSlot(this);
}

void UCUserWidget_BuffSelctionSlot::Unselected_Implementation()
{
}

UCUserWidget_BuffSelectHUD* UCUserWidget_BuffSelctionSlot::GetBuffListHUD() const
{
	UCGameInstance* instance = Cast<UCGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	CheckNullResult(instance, nullptr); 
	CheckNullResult(instance->BuffUIManager, nullptr); 

	return instance->BuffUIManager->GetBuffHUD();
}

