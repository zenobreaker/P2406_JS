
#include "SkillAssetEditor.h"
#include "EngineUtils.h"

#include "SSkillListView.h"
#include "SSkillDetails.h"
#include "SSkillInfoData.h"
#include "SSkillFlowData.h"
#include "SSkillPhaseData.h"
#include "SSkillCollisionDatas.h"
#include "SSkillEffect.h"

#include "Skill/CSkillAsset.h"

const FName FSkillAssetEditor::EditorName = "SkillAssetEditor";
const FName FSkillAssetEditor::ListViewTabName = "ListViewTab";
const FName FSkillAssetEditor::DetailsTabName = "DetailsTab";

const FName FSkillAssetEditor::PropertyEditorName = "PropertyEditor";


TSharedPtr<FSkillAssetEditor> FSkillAssetEditor::Instance = nullptr;


void FSkillAssetEditor::OpenWindow(FString InAssetName)
{
	if (Instance.IsValid())
	{
		if (Instance->ListView.IsValid())
		{
			FSkillRowDataPtr ptr = nullptr;

			if (InAssetName.IsEmpty() == false)
				ptr = Instance->ListView->FindRowDataPtrByName(InAssetName);

			if (ptr.IsValid() == false)
				ptr = Instance->ListView->GetFirstRowDataPtr();

			Instance->ListView->SetRowDataPtr(ptr->Asset);

			return;
		}


		Instance->CloseWindow(EAssetEditorCloseReason::AssetEditorHostClosed);

		Instance.Reset();
		Instance = nullptr;
	}

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


	{
		FDetailsViewArgs args;
		args.NameAreaSettings = FDetailsViewArgs::ENameAreaSettings::HideNameArea;
		args.ViewIdentifier = "SkillAssetEditorDetails";
		Details = propertyEditor.CreateDetailView(args);

		FOnGetDetailCustomizationInstance instance; 
		instance.BindStatic(SSkillDetails::MakeInstance);
		Details->SetGenericLayoutDetailsDelegate(instance);
	}


	//// Skill Effect - Property
	//{
	//	FString typeName = FSkillEffectInfo::StaticStruct()->GetName();

	//	FOnGetPropertyTypeCustomizationInstance instance;
	//	instance.BindStatic(SSkillEffect::MakeInstance);
	//	propertyEditor.RegisterCustomPropertyTypeLayout(FName(typeName), instance);
	//}

	// Skill Collision data - Property 
	{
		FString typeName = FSkillCollisionData::StaticStruct()->GetName();

		FOnGetPropertyTypeCustomizationInstance instance;
		instance.BindStatic(SSkillCollisionDatas::MakeInstance);
		propertyEditor.RegisterCustomPropertyTypeLayout(FName(typeName), instance);
	}


	// Skill Flow - Property
	{
		FString typeName = FSkillFlowData::StaticStruct()->GetName();

		FOnGetPropertyTypeCustomizationInstance instance;
		instance.BindStatic(SSkillFlowData::MakeInstance);
		propertyEditor.RegisterCustomPropertyTypeLayout(FName(typeName), instance);
	}

	// Skill Phase - Property
	{
		FString typeName = FSkillPhaseData::StaticStruct()->GetName();

		FOnGetPropertyTypeCustomizationInstance instance;
		instance.BindStatic(SSkillPhaseData::MakeInstance);
		propertyEditor.RegisterCustomPropertyTypeLayout(FName(typeName), instance);
	}

	// Skill Info 
	{
		FString typeName = FSkillInfo::StaticStruct()->GetName();

		FOnGetPropertyTypeCustomizationInstance instance; 
		instance.BindStatic(SSkillInfoData::MakeInstance);
		propertyEditor.RegisterCustomPropertyTypeLayout(FName(typeName), instance);
	}


	UCSkillAsset* asset = nullptr;


	TSharedRef<FTabManager::FLayout> layout = FTabManager::NewLayout("SkillAssetEditor_Layout")
		->AddArea
		(
			FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
			->Split
			(
				FTabManager::NewSplitter()->SetOrientation(Orient_Horizontal)
				->Split // ListView 
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.175)
					->AddTab(ListViewTabName, ETabState::OpenedTab)
					->SetHideTabWell(true)
				)
				->Split	// Details
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.725f)
					->AddTab(DetailsTabName, ETabState::OpenedTab)
					->SetHideTabWell(true)
				)
			)
		);

	ListView = SNew(SSkillListView)
		.OnSelectedItem(this, &FSkillAssetEditor::OnListViewSelectedItem);

	if (InAssetName.IsEmpty() == false)
	{
		FSkillRowDataPtr ptr = ListView->FindRowDataPtrByName(InAssetName);
		FSkillRowDataPtr selectedPtr = ListView->GetSelectedRowDataPtr();

		if (ptr == selectedPtr)
			return; 

		if (ptr.IsValid())
			asset = ptr->Asset;
	}

	if (asset == nullptr)
		asset = ListView->GetFirstRowDataPtr()->Asset;

	
	FAssetEditorToolkit::InitAssetEditor(EToolkitMode::Standalone, TSharedPtr<IToolkitHost>(), EditorName, layout, true, true, asset);

	ListView->SetRowDataPtr(asset); 
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

			propertyEditor.UnregisterCustomClassLayout(FName(UCSkillAsset::StaticClass()->GetName()));
			propertyEditor.UnregisterCustomClassLayout(FName(FSkillInfo::StaticStruct()->GetName()));
			propertyEditor.UnregisterCustomClassLayout(FName(FSkillFlowData::StaticStruct()->GetName()));
			propertyEditor.UnregisterCustomClassLayout(FName(FSkillPhaseData::StaticStruct()->GetName()));
			propertyEditor.UnregisterCustomClassLayout(FName(FSkillCollisionData::StaticStruct()->GetName()));
			propertyEditor.UnregisterCustomClassLayout(FName(FSkillEffectInfo::StaticStruct()->GetName()));
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
	return EditorName.ToString();
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

void FSkillAssetEditor::OnListViewSelectedItem(FSkillRowDataPtr InRowDataPtr)
{
	if (InRowDataPtr.IsValid() == false)
		return; 

	if (GetEditingObject() != nullptr)
		RemoveEditingObject(GetEditingObject());

	AddEditingObject(InRowDataPtr->Asset);
	Details->SetObject(InRowDataPtr->Asset);
}


TSharedRef<SDockTab> FSkillAssetEditor::Spawn_ListViewTab(const FSpawnTabArgs& InArgs)
{
	return SNew(SDockTab)
		[
			//SNew(SSkillListView)
			ListView.ToSharedRef()
		];

}

TSharedRef<SDockTab> FSkillAssetEditor::Spawn_DetailsTab(const FSpawnTabArgs& InArgs)
{
	return SNew(SDockTab)
		[
			Details.ToSharedRef()
		];
}
