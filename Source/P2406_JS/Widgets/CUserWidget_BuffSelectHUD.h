#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/UniformGridPanel.h"
#include "CUserWidget_BuffSelectHUD.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FShow_UI_BuffList);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHide_Ui_BuffList);

UCLASS()
class P2406_JS_API UCUserWidget_BuffSelectHUD : public UUserWidget
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UCUserWidget_BuffSelctionSlot> BuffSlotClass; 

public: 
	FORCEINLINE void SelectBuffSlot(class UCUserWidget_BuffSelctionSlot* InSlot) { SelectedBuffSlot = InSlot; }
	FORCEINLINE class UCUserWidget_BuffSelctionSlot* GetSelectedBuffSlot() { return SelectedBuffSlot; }
	FORCEINLINE UUniformGridPanel* GetGridPanel() { return GridPanel; }

public:
	virtual void NativeConstruct() override;

protected:
	UFUNCTION(BlueprintCallable)
	void AddChild(class UWidget* InSlot);

public: 
	void SetupBuffs(const TArray<struct FStatBuff>& InBuffs);
	

	UFUNCTION(BlueprintCallable)
	void ShowBuffHUD();
	
	UFUNCTION(BlueprintCallable)
	void HideBuffHUD();

public:
	FShow_UI_BuffList OnShow_UI_BuffList;
	FHide_Ui_BuffList OnHide_Ui_BuffList;


private:
	UUniformGridPanel* GridPanel;
	int32 ColumnCount = 0;
	class UCUserWidget_BuffSelctionSlot* SelectedBuffSlot = nullptr;
};
