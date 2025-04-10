#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

struct FSkillRowData
{
public:
	int Number; 
	FString Name;
	class UCSkillAsset* Asset; 

	FSkillRowData()
	{

	}

	FSkillRowData(int32 InNumber, FString InName, class UCSkillAsset* InAsset)
		:Number(InNumber), Name(InName), Asset(InAsset)
	{

	}

	static TSharedRef<FSkillRowData> Make(int32 InNumber, FString InName, class UCSkillAsset* InAsset)
	{
		return MakeShareable(new FSkillRowData(InNumber, InName, InAsset));
	}
};
typedef TSharedPtr<FSkillRowData> FSkillRowDataPtr;
//-----------------------------------------------------------------------------


class SKILLEDITOR_API SSkillTableRow
	: public SMultiColumnTableRow<FSkillRowDataPtr>
{
public:
	SLATE_BEGIN_ARGS(SSkillTableRow) {}
		SLATE_ARGUMENT(FSkillRowDataPtr, RowData)
	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& OwnerTableView);

protected:
	TSharedRef<SWidget> GenerateWidgetForColumn(const FName& InColumName) override;

private:
	FSkillRowDataPtr DataPtr; 
};


//-----------------------------------------------------------------------------

DECLARE_DELEGATE_OneParam(FOnSkillListViewSelctedItem, FSkillRowDataPtr)

class SKILLEDITOR_API SSkillListView
	: public SCompoundWidget
{

public:
	SLATE_BEGIN_ARGS(SSkillListView) {}
		SLATE_EVENT(FOnSkillListViewSelctedItem, OnSelectedItem) 
	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs);

private:
	TSharedRef<ITableRow> OnGenerateRow(FSkillRowDataPtr InRow, const TSharedRef<STableViewBase>& InTable);

private:
	FText OnGetAssetCount() const; 
	void OnTextChanged(const FText& InText); 
	void OnTextCommitted(const FText& InText, ETextCommit::Type);
	void OnSelectionChanged(FSkillRowDataPtr InDataPtr, ESelectInfo::Type InType);

public:
	FSkillRowDataPtr FindRowDataPtrByName(FString InAssetName); 

	bool HasRowDataPtr(); 
	FSkillRowDataPtr GetFirstRowDataPtr(); 

	FSkillRowDataPtr GetSelectedRowDataPtr(); 
	void SetRowDataPtr(class UCSkillAsset* InAsset); 

private:
	void ReadSkillAssetList(); 

private:
	TArray<FSkillRowDataPtr> RowDataPtrs;
	TSharedPtr<SListView<FSkillRowDataPtr>> ListView;

	TSharedPtr<SSearchBox> SearchBox; 
	FText SearchText; 

private:
	FOnSkillListViewSelctedItem OnSelectedItem; 
};
