#pragma once

#include "CoreMinimal.h"
#include "Toolkits/AssetEditorToolkit.h"


class WEAPONEDITOR_API FWeaponAssetEditor
	: public FAssetEditorToolkit
{
public:
	static void OpenWindow(FString InAssetName = ""); 
	static void Shutdown(); 

private:
	void Open(FString InAssetName); 

protected:
	bool OnRequestClose(EAssetEditorCloseReason InCloseReason) override;

private:
	static TSharedPtr<FWeaponAssetEditor> Instance; 

public:
	FLinearColor GetWorldCentricTabColorScale() const override;
	FName GetToolkitFName() const override;
	FText GetBaseToolkitName() const override;
	FString GetWorldCentricTabPrefix() const override;
	void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;

private:
	void OnListViewSelectedItem(FWeaponRowDataPtr InRowDataPtr);

private:
	TSharedRef<SDockTab> Spawn_ListViewTab(const FSpawnTabArgs& InArgs);
	TSharedRef<SDockTab> Spawn_DetailsTab(const FSpawnTabArgs& InArgs);

private:
	static const FName EditorName;
	static const FName ListViewTabName;
	static const FName DetailsTabName;

	static const FName PropertyEditorName;


private:
	TSharedPtr<class SWeaponListView> ListView;
	TSharedPtr<class IDetailsView> Details;
};
