#pragma once

#include "CoreMinimal.h"
#include "Toolkits/AssetEditorToolkit.h"

class SKILLEDITOR_API FSkillAssetEditor
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
	static TSharedPtr<FSkillAssetEditor> Instance;

public:
	FLinearColor GetWorldCentricTabColorScale() const override;
	FName GetToolkitFName() const override;
	FText GetBaseToolkitName() const override;
	FString GetWorldCentricTabPrefix() const override;
	void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;

private:
	TSharedRef<SDockTab> Spawn_ListViewTab(const FSpawnTabArgs& InArgs);
	TSharedRef<SDockTab> Spawn_DetailsTab(const FSpawnTabArgs& InArgs);



private:
	static const FName EditorName;
	static const FName ListViewTabName;
	static const FName DetailsTabName;

	static const FName PropertyEditorName;

private:
	TSharedPtr<class SSkillListView> ListView;
	TSharedPtr<class IDetailsView> Details;
};
