#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Datas/InputIconData.h"
#include "CUserWidget_SkillTimingPrompt.generated.h"


UCLASS()
class P2406_JS_API UCUserWidget_SkillTimingPrompt : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UFUNCTION(BlueprintCallable)
    void ShowInputPrompt(FKey InputKey);  // 스킬에서 이걸 호출

    UFUNCTION(BlueprintCallable)
    void HideInputPrompt();

private:
    void UpdateIcon(FKey Key);

protected:
    UPROPERTY(meta = (BindWidget))
    class UImage* InputIcon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UDataTable* InputIconTable;

};
