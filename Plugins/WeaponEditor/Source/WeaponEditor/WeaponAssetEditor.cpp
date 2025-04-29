#include "WeaponAssetEditor.h"
#include "EngineUtils.h"



#include "SWeaponDetails.h"
#include "SWeaponEquipmentData.h"
#include "SWeaponDoActionDatas.h"
#include "SWeaponDamagedDatas.h"


#include "Weapons/CWeaponAsset.h"


const FName FWeaponAssetEditor::EditorName = "SkillAssetEditor";
const FName FWeaponAssetEditor::ListViewTabName = "ListViewTab";
const FName FWeaponAssetEditor::DetailsTabName = "DetailsTab";

const FName FWeaponAssetEditor::PropertyEditorName = "PropertyEditor";


TSharedPtr<FWeaponAssetEditor> FWeaponAssetEditor::Instance = nullptr;


void FWeaponAssetEditor::OpenWindow(FString InAssetName)
{
	if (Instance.IsValid())
	{
		if (Instance->ListView.IsValid())
		{
			FWeaponRowDataPtr ptr = nullptr;

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

	Instance = MakeShareable(new FWeaponAssetEditor());
	Instance->Open(InAssetName);
}

void FWeaponAssetEditor::Shutdown()
{
	if (Instance.IsValid())
	{
		Instance->CloseWindow(EAssetEditorCloseReason::AssetEditorHostClosed);

		Instance.Reset();
		Instance = nullptr;
	}
}

void FWeaponAssetEditor::Open(FString InAssetName)
{
	FPropertyEditorModule& propertyEditor = FModuleManager::LoadModuleChecked<FPropertyEditorModule>(PropertyEditorName);


	//UCWeaponAsset - Details
	{
		FDetailsViewArgs args;
		args.NameAreaSettings = FDetailsViewArgs::ENameAreaSettings::HideNameArea;
		args.ViewIdentifier = "WeaponAssetEditorDetails";
		Details = propertyEditor.CreateDetailView(args);

		FOnGetDetailCustomizationInstance instance;
		instance.BindStatic(SWeaponDetails::MakeInstance);
		Details->SetGenericLayoutDetailsDelegate(instance);
	}

	//EquipmentData - Property
	{
		FString typeName = FEquipmentData::StaticStruct()->GetName();

		FOnGetPropertyTypeCustomizationInstance instance;
		instance.BindStatic(&SWeaponEquipmentData::MakeInstance);
		propertyEditor.RegisterCustomPropertyTypeLayout(FName(typeName), instance);
	}

	//DoActionDatas - Property
	{
		FString typeName = FDoActionData::StaticStruct()->GetName();

		FOnGetPropertyTypeCustomizationInstance instance;
		instance.BindStatic(&SWeaponDoActionDatas::MakeInstance);
		propertyEditor.RegisterCustomPropertyTypeLayout(FName(typeName), instance);
	}

	//DamagedDatas - Property
	{
		FString typeName = FHitData::StaticStruct()->GetName();

		FOnGetPropertyTypeCustomizationInstance instance;
		instance.BindStatic(&SWeaponDamagedDatas::MakeInstance);
		propertyEditor.RegisterCustomPropertyTypeLayout(FName(typeName), instance);
	}



	UCWeaponAsset* asset = nullptr;

	TSharedRef<FTabManager::FLayout> layout = FTabManager::NewLayout("WeaponAssetEditor_Layout")
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

	ListView = SNew(SWeaponListView)
		.OnSelectedItem(this, &FWeaponAssetEditor::OnListViewSelectedItem);

	if (InAssetName.IsEmpty() == false)
	{
		FWeaponRowDataPtr ptr = ListView->FindRowDataPtrByName(InAssetName);
		FWeaponRowDataPtr selctedPtr = ListView->GetSelectedRowDataPtr();

		if (ptr == selctedPtr)
			return;

		if (ptr.IsValid())
			asset = ptr->Asset;
	}

	if (asset == nullptr)
		asset = ListView->GetFirstRowDataPtr()->Asset;

	
	FAssetEditorToolkit::InitAssetEditor(EToolkitMode::Standalone, TSharedPtr<IToolkitHost>(), EditorName, layout, true, true, asset);

	ListView->SetRowDataPtr(asset);
}

bool FWeaponAssetEditor::OnRequestClose(EAssetEditorCloseReason InCloseReason)
{
	if (Details != nullptr)
	{
		if (!!GEditor && !!GEditor->GetEditorSubsystem<UAssetEditorSubsystem>())
			GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->NotifyAssetClosed(GetEditingObject(), this);

		if (FModuleManager::Get().IsModuleLoaded(PropertyEditorName))
		{
			FPropertyEditorModule& propertyEditor = FModuleManager::LoadModuleChecked<FPropertyEditorModule>(PropertyEditorName);
			propertyEditor.UnregisterCustomPropertyTypeLayout(FName(FEquipmentData::StaticStruct()->GetName()));
			propertyEditor.UnregisterCustomPropertyTypeLayout(FName(FDoActionData::StaticStruct()->GetName()));
			propertyEditor.UnregisterCustomPropertyTypeLayout(FName(FHitData::StaticStruct()->GetName()));

			propertyEditor.UnregisterCustomClassLayout(FName(UCWeaponAsset::StaticClass()->GetName()));
		}
	}

	if (ListView.IsValid())
		ListView.Reset();

	if (Details.IsValid())
		Details.Reset();


	return true;
}

FLinearColor FWeaponAssetEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor(0, 0, 1);
}

FName FWeaponAssetEditor::GetToolkitFName() const
{
	return EditorName;
}

FText FWeaponAssetEditor::GetBaseToolkitName() const
{
	return FText::FromName(EditorName);
}

FString FWeaponAssetEditor::GetWorldCentricTabPrefix() const
{
	return EditorName.ToString();
}

void FWeaponAssetEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	FOnSpawnTab listViewTab;
	listViewTab.BindSP(this, &FWeaponAssetEditor::Spawn_ListViewTab);
	TabManager->RegisterTabSpawner(ListViewTabName, listViewTab);


	FOnSpawnTab detailsTab;
	detailsTab.BindSP(this, &FWeaponAssetEditor::Spawn_DetailsTab);
	TabManager->RegisterTabSpawner(DetailsTabName, detailsTab);
}

void FWeaponAssetEditor::OnListViewSelectedItem(FWeaponRowDataPtr InRowDataPtr)
{
	if (InRowDataPtr.IsValid() == false)
		return;

	if (GetEditingObject() != nullptr)
		RemoveEditingObject(GetEditingObject());

	AddEditingObject(InRowDataPtr->Asset);
	Details->SetObject(InRowDataPtr->Asset);
}

TSharedRef<SDockTab> FWeaponAssetEditor::Spawn_ListViewTab(const FSpawnTabArgs& InArgs)
{
	return SNew(SDockTab)
		[
			ListView.ToSharedRef()
		];

}

TSharedRef<SDockTab> FWeaponAssetEditor::Spawn_DetailsTab(const FSpawnTabArgs& InArgs)
{
	return SNew(SDockTab)
		[
			Details.ToSharedRef()
		];
}
