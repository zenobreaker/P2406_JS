#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Buffs/CBuffStructures.h"
#include "CUserWidget_BuffSelctionSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUserWidget_Buff_Clicked);

UCLASS()
class P2406_JS_API UCUserWidget_BuffSelctionSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = "Stat")
	struct FStatBuff StatBuff;

public:
	FORCEINLINE struct  FStatBuff GetStatBuff() const { return StatBuff; }

protected:
	UFUNCTION(BlueprintCallable)
	void Clicked(); 

	UFUNCTION(BlueprintCallable)
	void Hovered(); 

	UFUNCTION(BlueprintCallable)
	void Unhovered(); 

public:
	UFUNCTION(BlueprintNativeEvent, Category = "UserInterface")
	void SetBuff(const struct FStatBuff& InBuff, const struct FStatBuffUIData& InUIData);
	virtual void SetBuff_Implementation(const struct FStatBuff& InBuff,  const struct FStatBuffUIData& InUIData);

	UFUNCTION(BlueprintNativeEvent, Category = "UserInterface")
	void Selected(); 
	virtual void Selected_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "UserInterface")
	void Unselected(); 
	virtual void Unselected_Implementation();

private: 
	class UCUserWidget_BuffSelectHUD* GetBuffListHUD() const;

public:
	FUserWidget_Buff_Clicked OnClicked;


};
