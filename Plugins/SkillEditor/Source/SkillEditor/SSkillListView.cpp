#include "SSkillListView.h"
#include "EngineUtils.h"
#include "Widgets/Input/SSearchBox.h"

#include "Skill/CSkillAsset.h"

void SSkillTableRow::Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& OwnerTableView)
{
	DataPtr = InArgs._RowData;

	SMultiColumnTableRow<FSkillRowDataPtr>::Construct(FSuperRowType::FArguments(), OwnerTableView);
}

TSharedRef<SWidget> SSkillTableRow::GenerateWidgetForColumn(const FName& InColumName)
{

	FString str;
	if (InColumName == "Number")
		str = FString::FromInt(DataPtr->Number);
	else if (InColumName == "Name")
		str = DataPtr->Name;

	return SNew(STextBlock)
		.Text(FText::FromString(str));
}

//-----------------------------------------------------------------------------

void SSkillListView::Construct(const FArguments& InArgs)
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
						.OnTextChanged(this, &SSkillListView::OnTextChanged)
						.OnTextCommitted(this, &SSkillListView::OnTextCommitted)
				]
				+ SVerticalBox::Slot()
				.FillHeight(1)
				[
					SAssignNew(ListView, SListView<FSkillRowDataPtr>)
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
						.OnGenerateRow(this, &SSkillListView::OnGenerateRow)
						.SelectionMode(ESelectionMode::Single)
						.OnSelectionChanged(this, &SSkillListView::OnSelectionChanged)
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Right)
				.Padding(8, 2)
				[
					SNew(STextBlock)
						.Text(this, &SSkillListView::OnGetAssetCount)
				]
		];

	ReadSkillAssetList();
}

TSharedRef<ITableRow> SSkillListView::OnGenerateRow(FSkillRowDataPtr InRow, const TSharedRef<STableViewBase>& InTable)
{
	return SNew(SSkillTableRow, InTable)
		.RowData(InRow);
}

FText SSkillListView::OnGetAssetCount() const
{
	FString str = "";
	str += FString::Printf(L"%d Finds", RowDataPtrs.Num());

	return FText::FromString(str);
}

void SSkillListView::OnTextChanged(const FText& InText)
{
	if (SearchText.CompareToCaseIgnored(InText) == 0)
		return;

	SearchText = InText;
	ReadSkillAssetList();
}

void SSkillListView::OnTextCommitted(const FText& InText, ETextCommit::Type)
{
	OnTextChanged(InText); 
}

void SSkillListView::OnSelectionChanged(FSkillRowDataPtr InDataPtr, ESelectInfo::Type InType)
{
	if (InDataPtr.IsValid() == false)
		return;

	OnSelectedItem.ExecuteIfBound(InDataPtr);
}

FSkillRowDataPtr SSkillListView::FindRowDataPtrByName(FString InAssetName)
{
	for (FSkillRowDataPtr ptr : RowDataPtrs)
	{
		if (ptr->Name == InAssetName)
			return ptr; 
	}

	return nullptr; 
}

bool SSkillListView::HasRowDataPtr()
{
	return RowDataPtrs.Num() > 0;
}

FSkillRowDataPtr SSkillListView::GetFirstRowDataPtr()
{
	ensure(HasRowDataPtr() == true);

	return RowDataPtrs[0];
}

FSkillRowDataPtr SSkillListView::GetSelectedRowDataPtr()
{
	TArray<FSkillRowDataPtr> ptrs = ListView->GetSelectedItems();

	if (ptrs.Num() > 0)
		return ptrs[0];

	return nullptr;
}

void SSkillListView::SetRowDataPtr(UCSkillAsset* InAsset)
{
	if (HasRowDataPtr() == false)
		return;

	for (FSkillRowDataPtr ptr : RowDataPtrs)
	{
		if (ptr->Asset == InAsset)
		{
			ListView->SetSelection(ptr);

			return; 
		}
	}
}

void SSkillListView::ReadSkillAssetList()
{
	RowDataPtrs.Empty(); 

	TArray<UObject*> objects;
	EngineUtils::FindOrLoadAssetsByPath("/Game/Skills/", objects, EngineUtils::ATL_Regular);

	int32 index = 0; 
	for (UObject* obj : objects)
	{
		UCSkillAsset* skill = Cast<UCSkillAsset>(obj);
		if (skill == nullptr ) return;

		FString name = skill->GetName(); 
		if (SearchText.IsEmpty() == false)
		{
			if (name.Contains(SearchText.ToString()) == false)
				continue;
		}

		RowDataPtrs.Add(FSkillRowData::Make(++index, name, skill));
	}

	RowDataPtrs.Sort([](const FSkillRowDataPtr& A, const FSkillRowDataPtr& B)
		{
			return A->Name < B->Name;
		});

	ListView->RequestListRefresh();
}
