#include "SWeaponListView.h"
#include "EngineUtils.h"
#include "Widgets/Input/SSearchBox.h"

#include "Weapons/CWeaponAsset.h"

void SWeaponTableRow::Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& OwnerTableView)
{
	DataPtr = InArgs._RowData;

	SMultiColumnTableRow<FWeaponRowDataPtr>::Construct(FSuperRowType::FArguments(), OwnerTableView);
}

TSharedRef<SWidget> SWeaponTableRow::GenerateWidgetForColumn(const FName& InColumName)
{
	FString str; 
	if (InColumName == "Number")
		str = FString::FromInt(DataPtr->Number);
	else if (InColumName == "Name")
		str = DataPtr->Name; 

	return SNew(STextBlock)
		.Text(FText::FromString(str));
}

//-------------------------------------------------------------------------------

void SWeaponListView::Construct(const FArguments& InArgs)
{
	OnSelectedItem = InArgs._OnSelectedItem;

	ChildSlot
		[
			SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(2, 0)
				[
					SAssignNew(SearchBox, SSearchBox)
						.SelectAllTextWhenFocused(true)
						.OnTextChanged(this, &SWeaponListView::OnTextChanged)
						.OnTextCommitted(this, &SWeaponListView::OnTextCommitted)
				]
				+ SVerticalBox::Slot()
				.FillHeight(1)
				[
					SAssignNew(ListView, SListView<FWeaponRowDataPtr>)
						.HeaderRow
						(
							SNew(SHeaderRow)
							+ SHeaderRow::Column("Number")
							.DefaultLabel(FText::FromString(""))
							.ManualWidth(40)
							+ SHeaderRow::Column("Name")
							.DefaultLabel(FText::FromString("Name"))
						)
						.ListItemsSource(&RowDataPtrs)
						.OnGenerateRow(this, &SWeaponListView::OnGenerateRow)
						.SelectionMode(ESelectionMode::Single)
						.OnSelectionChanged(this, &SWeaponListView::OnSelectionChanged)
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Right)
				.Padding(0, 2)
				[
					SNew(STextBlock)
						.Text(this, &SWeaponListView::OnGetAssetCount)
				]
		];
		
	ReadWeaponAssetList();
}

TSharedRef<ITableRow> SWeaponListView::OnGenerateRow(FWeaponRowDataPtr InRow, const TSharedRef<STableViewBase>& InTable)
{
	return SNew(SWeaponTableRow, InTable)
		.RowData(InRow);
}

FText SWeaponListView::OnGetAssetCount() const
{
	FString str = "";
	str += FString::Printf(L"%d Finds", RowDataPtrs.Num());

	return FText::FromString(str);
}

void SWeaponListView::OnTextChanged(const FText& InText)
{
	if (SearchText.CompareToCaseIgnored(InText) == 0)
		return;

	SearchText = InText;
	ReadWeaponAssetList();
}

void SWeaponListView::OnTextCommitted(const FText& InText, ETextCommit::Type)
{
	OnTextChanged(InText);
}

void SWeaponListView::OnSelectionChanged(FWeaponRowDataPtr InDataPtr, ESelectInfo::Type InType)
{
	if (InDataPtr.IsValid() == false)
		return; 

	OnSelectedItem.ExecuteIfBound(InDataPtr);
}

FWeaponRowDataPtr SWeaponListView::FindRowDataPtrByName(FString InAssetName)
{
	for (FWeaponRowDataPtr ptr : RowDataPtrs)
	{
		if (ptr->Name == InAssetName)
			return ptr; 
	}

	return nullptr;
}

bool SWeaponListView::HasRowDataPtr()
{
	return RowDataPtrs.Num() > 0;
}

FWeaponRowDataPtr SWeaponListView::GetFirstRowDataPtr()
{
	ensure(HasRowDataPtr() == true);

	return RowDataPtrs[0];
}

FWeaponRowDataPtr SWeaponListView::GetSelectedRowDataPtr()
{
	TArray<FWeaponRowDataPtr> ptrs = ListView->GetSelectedItems(); 

	if (ptrs.Num() > 0)
		return ptrs[0];

	return nullptr;
}

void SWeaponListView::SetRowDataPtr(UCWeaponAsset* InAsset)
{
	if (HasRowDataPtr() == false)
		return;

	for (FWeaponRowDataPtr ptr  : RowDataPtrs)
	{
		if (ptr->Asset == InAsset)
		{
			ListView->SetSelection(ptr);

			return;
		}
	}
}

void SWeaponListView::ReadWeaponAssetList()
{
	RowDataPtrs.Empty();

	TArray<UObject*> objects;
	EngineUtils::FindOrLoadAssetsByPath("/Game/", objects, EngineUtils::ATL_Regular);

	int32 index = 0;
	for (UObject* obj : objects)
	{
		UCWeaponAsset* skill = Cast<UCWeaponAsset>(obj);
		if (skill == nullptr) continue;

		FString name = skill->GetName();
		if (SearchText.IsEmpty() == false)
		{
			if (name.Contains(SearchText.ToString()) == false)
				continue;
		}

		RowDataPtrs.Add(FWeaponRowData::Make(++index, name, skill));
	}

	RowDataPtrs.Sort([](const FWeaponRowDataPtr& A, const FWeaponRowDataPtr& B)
		{
			return A->Name < B->Name;
		});

	ListView->RequestListRefresh();
}
