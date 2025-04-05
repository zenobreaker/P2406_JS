#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Buffs/CBuffStructures.h"
#include "CBuffUIManager.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FShowedUIBuffList);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHidedUIBuffList);

UCLASS(Blueprintable)
class P2406_JS_API UCBuffUIManager : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCUserWidget_BuffSelectHUD> BuffSelectHUDClass;

	UPROPERTY(EditAnywhere)
	UDataTable* StatBuffUITable;

public:
	FORCEINLINE TSubclassOf<class UCUserWidget_BuffSelectHUD> GetBuffHUDClass() { return BuffSelectHUDClass; }
	FORCEINLINE void SetBuffHUDClass(TSubclassOf<class UCUserWidget_BuffSelectHUD> InClass) { BuffSelectHUDClass = InClass; }
	FORCEINLINE class UCUserWidget_BuffSelectHUD* GetBuffHUD() { return BuffSelectHUD; }

public:
	void BeginPlay(UWorld* InWorld);
	void ShowBuffSelection(const TArray<struct FStatBuff>& Buffs);
	void HideBuffSelection();

	struct FStatBuff GetSelectedStatBuff();

private:
	void GetBuffUiDatas(const TArray<struct FStatBuff>& InBuffs, TArray<struct FStatBuffUIData>& OutUiDatas);

private:
	UFUNCTION()
	void OnShowUIBuffList();

	UFUNCTION()
	void OnHideUIBuffList();

public:
	FShowedUIBuffList OnShowedUIBuffList;
	FHidedUIBuffList OnHidedUIBuffList;

private:
	class UCUserWidget_BuffSelectHUD* BuffSelectHUD;
	UWorld* World;
private:
	TMap<FName,struct FStatBuffUIData> UIDatas;
};
