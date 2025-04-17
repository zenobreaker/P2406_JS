#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"


struct FWeaponRowData
{
public:
	int Number; 
	FString Name; 
	class UCWeaponAsset* Asset;

	FWeaponRowData() {}

	FWeaponRowData(int32 InNumber, FString InName, class UCWeaponAsset* InAsset)
		:Number(InNumber), Name(InName), Asset(InAsset)
	{

	}

	static TSharedRef<FWeaponRowData> Make(int32 InNumber, FString InName, class UCWeaponAsset* InAsset)
	{
		return MakeShareable(new FWeaponRowData(InNumber, InName, InAsset));
	}
};
typedef TSharedPtr<FWeaponRowData> FWeaponRowDataPtr;


class WEAPONEDITOR_API SWeaponTableRow
	: public SMultiColumnTableRow<FWeaponRowDataPtr>
{
public:
	SLATE_BEGIN_ARGS(SWeaponTableRow) {}
		SLATE_ARGUMENT(FWeaponRowDataPtr, RowData)
	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& OwnerTableView);

protected:
	TSharedRef<SWidget> GenerateWidgetForColumn(const FName& InColumName) override;

private:
	FWeaponRowDataPtr DataPtr;
};

DECLARE_DELEGATE_OneParam(FOnWeaponListViewSelctedItem, FWeaponRowDataPtr)

class WEAPONEDITOR_API SWeaponListView
	: public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SWeaponListView) {}
		SLATE_EVENT(FOnWeaponListViewSelctedItem, OnSelectedItem)
	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs);

private:
	TSharedRef<ITableRow> OnGenerateRow(FWeaponRowDataPtr InRow, const TSharedRef<STableViewBase>& InTable);

private:
	FText OnGetAssetCount() const; 
	void OnTextChanged(const FText& InText);
	void OnTextCommitted(const FText& InText, ETextCommit::Type); 
	void OnSelectionChanged(FWeaponRowDataPtr InDataPtr, ESelectInfo::Type InType);

public:
	FWeaponRowDataPtr FindRowDataPtrByName(FString InAssetName); 

	bool HasRowDataPtr();
	FWeaponRowDataPtr GetFirstRowDataPtr(); 

	FWeaponRowDataPtr GetSelectedRowDataPtr(); 
	void SetRowDataPtr(class UCWeaponAsset* InAsset); 

private:
	void ReadWeaponAssetList();

private:
	TArray<FWeaponRowDataPtr> RowDataPtrs;
	TSharedPtr<SListView<FWeaponRowDataPtr>> ListView; 

	TSharedPtr<SSearchBox> SearchBox; 
	FText SearchText; 

private:
	FOnWeaponListViewSelctedItem OnSelectedItem; 
};
