#include "SkillAssetEditor.h"
#include "EngineUtils.h"



#include "Skill/CSkillAsset.h"

const FName FSkillAssetEditor::EditorName = "SkillAssetEditor";
const FName FSkillAssetEditor::ListViewTabName = "ListViewTab";
const FName FSkillAssetEditor::DetailsTabName = "DetailsTab";

const FName FSkillAssetEditor::PropertyEditorName = "PropertyEditor";


TSharedPtr<FSkillAssetEditor> FSkillAssetEditor::Instance = nullptr;


void FSkillAssetEditor::OpenWindow(FString InAssetName)
{
	//if (Instance.IsValid())
	//{
	//	if (Instance->ListView.IsValid())
	//	{
	//		//FWeaponRowDataPtr ptr = nullptr;

	//		if (InAssetName.IsEmpty() == false)
	//			ptr = Instance->ListView->FindRowDataPtrByName(InAssetName);

	//		if (ptr.IsValid() == false)
	//			ptr = Instance->ListView->GetFirstRowDataPtr();

	//		Instance->ListView->SetRowDataPtr(ptr->Asset);

	//		return;
	//	}


	//	Instance->CloseWindow(EAssetEditorCloseReason::AssetEditorHostClosed);

	//	Instance.Reset();
	//	Instance = nullptr;
	//}

	Instance = MakeShareable(new FSkillAssetEditor());
	Instance->Open(InAssetName);
}

void FSkillAssetEditor::Shutdown()
{
	if (Instance.IsValid())
	{
		Instance->CloseWindow(EAssetEditorCloseReason::AssetEditorHostClosed);

		Instance.Reset();
		Instance = nullptr;
	}
}

void FSkillAssetEditor::Open(FString InAssetName)
{
	FPropertyEditorModule& propertyEditor = FModuleManager::LoadModuleChecked<FPropertyEditorModule>(PropertyEditorName);



	UCSkillAsset* asset = nullptr;

	
}

bool FSkillAssetEditor::OnRequestClose(EAssetEditorCloseReason InCloseReason)
{
	if (!!Details)
	{
		if (!!GEditor && !!GEditor->GetEditorSubsystem<UAssetEditorSubsystem>())
			GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->NotifyAssetClosed(GetEditingObject(), this);

		if (FModuleManager::Get().IsModuleLoaded(PropertyEditorName))
		{
			FPropertyEditorModule& propertyEditor = FModuleManager::LoadModuleChecked<FPropertyEditorModule>(PropertyEditorName);
			//propertyEditor.UnregisterCustomPropertyTypeLayout("EquipmentData");
			//propertyEditor.UnregisterCustomPropertyTypeLayout("DoActionData");
			//propertyEditor.UnregisterCustomPropertyTypeLayout("DamageData");

			propertyEditor.UnregisterCustomClassLayout(FName(UCSkillAsset::StaticClass()->GetName()));
		}
	}


	if (ListView.IsValid())
		ListView.Reset();

	if (Details.IsValid())
		Details.Reset();

	return true;
}

FLinearColor FSkillAssetEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor(0, 0, 1);
}

FName FSkillAssetEditor::GetToolkitFName() const
{
	return EditorName;
}

FText FSkillAssetEditor::GetBaseToolkitName() const
{
	return FText::FromName(EditorName);
}

FString FSkillAssetEditor::GetWorldCentricTabPrefix() const
{
	return  EditorName.ToString();
}

void FSkillAssetEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	FOnSpawnTab listViewTab;
	listViewTab.BindSP(this, &FSkillAssetEditor::Spawn_ListViewTab);
	TabManager->RegisterTabSpawner(ListViewTabName, listViewTab);

	FOnSpawnTab detailsTab;
	detailsTab.BindSP(this, &FSkillAssetEditor::Spawn_DetailsTab);
	TabManager->RegisterTabSpawner(DetailsTabName, detailsTab);
}


TSharedRef<SDockTab> FSkillAssetEditor::Spawn_ListViewTab(const FSpawnTabArgs& InArgs)
{
	//return SNew(SDockTab)
	//[
	//	SNew(SBox)
	//	[
	//		SNew(SButton)
	//		[
	//			SNew(STextBlock)
	//			.Text(FText::FromString("Test"))
	//		]
	//	]
	//];

	//return SNew(SDockTab)
	//	[
	//		//SNew(SWeaponListView)
	//		ListView.ToSharedRef()
	//	];

	return SNew(SDockTab);
}

TSharedRef<SDockTab> FSkillAssetEditor::Spawn_DetailsTab(const FSpawnTabArgs& InArgs)
{
	//TSharedPtr<SBox> box;
	//SAssignNew(box, SBox)
	//[
	//	SNew(SButton)
	//	[
	//		SNew(STextBlock)
	//		.Text(FText::FromString("Test3"))
	//	]
	//];	

	//return SNew(SDockTab)
	//[
	//	box.ToSharedRef()
	//];

	return SNew(SDockTab)
		[
			Details.ToSharedRef()
		];
}
