#include "Widgets/CUserWidget_SkillTimingPrompt.h"
#include "Global.h"
#include "Components/Image.h"


void UCUserWidget_SkillTimingPrompt::ShowInputPrompt(FKey InputKey)
{
	UpdateIcon(InputKey);
	SetVisibility(ESlateVisibility::Visible);
}

void UCUserWidget_SkillTimingPrompt::HideInputPrompt()
{
    SetVisibility(ESlateVisibility::Hidden);
}

void UCUserWidget_SkillTimingPrompt::UpdateIcon(FKey Key)
{
    static const FString Context = TEXT("InputIconLookup");
    if (InputIconTable)
    {
        FInputIconData* Row = InputIconTable->FindRow<FInputIconData>(Key.GetFName(), Context);
        if (Row && Row->IconImage)
        {
            InputIcon->SetBrushFromTexture(Row->IconImage);
        }
    }
}
