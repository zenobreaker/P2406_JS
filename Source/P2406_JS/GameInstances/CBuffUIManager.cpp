#include "CBuffUIManager.h"
#include "Global.h"
#include "Widgets/CUserWidget_BuffSelectHUD.h"
#include "Widgets/CUserWidget_BuffSelctionSlot.h"

void UCBuffUIManager::BeginPlay(UWorld* InWorld)
{
	CheckNull(InWorld); 
	CheckNull(BuffSelectHUDClass);

}

void UCBuffUIManager::ShowBuffSelection(const TArray<struct FStatBuff>& Buffs)
{
	if (BuffSelectHUD == nullptr && BuffSelectHUDClass != nullptr)
	{
		BuffSelectHUD = CreateWidget<UCUserWidget_BuffSelectHUD>(GetWorld()->GetFirstPlayerController(), BuffSelectHUDClass);
		if (!!BuffSelectHUD)
		{
			REGISTER_EVENT_WITH_REPLACE(BuffSelectHUD, OnShow_UI_BuffList, this, UCBuffUIManager::OnShowUIBuffList);
			REGISTER_EVENT_WITH_REPLACE(BuffSelectHUD, OnHide_Ui_BuffList, this, UCBuffUIManager::OnHideUIBuffList);


			BuffSelectHUD->AddToViewport();
		}
	}
	
	CheckNull(BuffSelectHUD);

	BuffSelectHUD->SetupBuffs(Buffs); 
	BuffSelectHUD->ShowBuffHUD();
}

void UCBuffUIManager::HideBuffSelection()
{
	CheckNull(BuffSelectHUD);

	BuffSelectHUD->HideBuffHUD();
}

FStatBuff UCBuffUIManager::GetSelectedStatBuff()
{
	CheckNullResult(BuffSelectHUD, FStatBuff());
	CheckNullResult(BuffSelectHUD->GetSelectedBuffSlot(), FStatBuff());

	UCUserWidget_BuffSelctionSlot* slot = BuffSelectHUD->GetSelectedBuffSlot();

	return slot->GetStatBuff();
}

void UCBuffUIManager::OnShowUIBuffList()
{
	if (OnShowedUIBuffList.IsBound())
		OnShowedUIBuffList.Broadcast();
}

void UCBuffUIManager::OnHideUIBuffList()
{
	if (OnHidedUIBuffList.IsBound())
		OnHidedUIBuffList.Broadcast();
}
