#include "SSkillCheckBoxes.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "IPropertyUtilities.h"
#include "Widgets/Layout/SUniformGridPanel.h"

#include "SSkillDetails.h"
#if WITH_EDITOR
#include "Weapons/CWeaponStructures.h"
#include "Skill/CSkillStructures.h"
#include "Skill/CSkillEntity.h"
#endif

void SSkillCheckBoxes::AddProperties(TSharedPtr<IPropertyHandle> InHandle)
{
	uint32 count = 0; 
	InHandle->GetNumChildren(count);

	for (uint32 i = 0; i < count; i++)
	{
		TSharedPtr<IPropertyHandle> child = InHandle->GetChildHandle(i);

		InternalDatas.Add(child); 
	}
}

bool SSkillCheckBoxes::CanDraw(TSharedPtr<class IPropertyHandle> InHandle, int32 InCount)
{
	bool b = true;
	b &= InCount > 0;

	int32 index = InHandle->GetIndexInArray();
	b &= index >= 0;
	b &= index < InCount;

	return b;
}

TSharedRef<SWidget> SSkillCheckBoxes::DrawHeader()
{
	TSharedPtr<SUniformGridPanel> panel;
	SAssignNew(panel, SUniformGridPanel);
	panel->SetMinDesiredSlotWidth(150);

	for (int32 i = 0; i < InternalDatas.Num(); i++)
	{
		panel->AddSlot(i, 0)
			[
				SNew(SCheckBox)
					.IsChecked(InternalDatas[i].bChecked)
					.OnCheckStateChanged(this, &SSkillCheckBoxes::OnCheckStateChanged, i)
					[
						SNew(STextBlock)
							.Text(FText::FromString(InternalDatas[i].Name))
					]
			];
	}

	return panel.ToSharedRef();
}

void SSkillCheckBoxes::DrawProperties(TSharedRef<IPropertyHandle> InPropertyHandle, IDetailChildrenBuilder* InChildBuilder)
{
	for (int32 i = 0; i < InternalDatas.Num(); i++)
	{
		if (InternalDatas[i].bChecked == false)
			continue;


		TSharedPtr<IPropertyHandle> child = InPropertyHandle->GetChildHandle(i);
		IDetailPropertyRow& row = InChildBuilder->AddProperty(child.ToSharedRef());

		FText value;
		child->GetValueAsDisplayText(value);

		row.CustomWidget()
			.NameContent()
			[
				child->CreatePropertyNameWidget()
			]
			.ValueContent()
			.MinDesiredWidth(300)
			[
				child->CreatePropertyValueWidget()
			];
	}
}

void SSkillCheckBoxes::CheckDefaultObject(int32 InIndex, UObject* InDefaultValue)
{
	UObject* value = nullptr;
	InternalDatas[InIndex].Handle->GetValue(value);

	if (!!value && InDefaultValue != value)
		InternalDatas[InIndex].bChecked = true;
}

void SSkillCheckBoxes::CheckDefaultValue(int32 InIndex, float InDefaultValue)
{
	float value = 0.0f;
	InternalDatas[InIndex].Handle->GetValue(value);
	
	if (InDefaultValue != value)
		InternalDatas[InIndex].bChecked = true;
}

void SSkillCheckBoxes::CheckDefaultValue(int32 InIndex, int32 InDefaultValue)
{
	int32 value = 0; 
	InternalDatas[InIndex].Handle->GetValue(value); 

	if (InDefaultValue != value)
		InternalDatas[InIndex].bChecked = true;
}

void SSkillCheckBoxes::CheckDefaultValue(int32 InIndex, bool InDefaultValue)
{
	bool value = false;
	InternalDatas[InIndex].Handle->GetValue(value);

	if (InDefaultValue != value)
		InternalDatas[InIndex].bChecked = true;
}

void SSkillCheckBoxes::CheckDefaultValue(int32 InIndex, const FVector& InDefaultValue)
{
	FVector value = FVector::ZeroVector;
	InternalDatas[InIndex].Handle->GetValue(value);

	if (InDefaultValue != value)
		InternalDatas[InIndex].bChecked = true;
}

void SSkillCheckBoxes::CheckDefaultValue(int32 InIndex, const FRotator& InDefaultValue)
{
	FRotator value = FRotator::ZeroRotator;
	InternalDatas[InIndex].Handle->GetValue(value);

	if (InDefaultValue != value)
		InternalDatas[InIndex].bChecked = true;
}

void SSkillCheckBoxes::CheckDefaultValue(int32 InIndex, const FString& InDefaultValue)
{
	FString value = FString();
	InternalDatas[InIndex].Handle->GetValue(value);

	if (InDefaultValue != value)
		InternalDatas[InIndex].bChecked = true;
}

void SSkillCheckBoxes::CheckDefaultDoActionData(int32 InIndex, const FDoActionData& InDefautVale)
{
	FDoActionData value = FDoActionData();
	
	bool bCheck = false; 
	if (value.Montage != InDefautVale.Montage)
		bCheck = true;
	if (value.PlayRate != InDefautVale.PlayRate)
		bCheck = true; 
	if (value.bCanMove != InDefautVale.bCanMove)
		bCheck = true; 
	if (value.bFixedCamera != InDefautVale.bFixedCamera)
		bCheck = true; 

	if (bCheck == true)
		InternalDatas[InIndex].bChecked = true;
}

void SSkillCheckBoxes::CheckDefaultSkillEntityData(int32 InIndex, const FSkillEntityData& InDefaultValue)
{

	bool bCheck = false;
	if (InDefaultValue.SkillEntity != nullptr)
		bCheck = true; 
	if (InDefaultValue.SpawnLocation != FVector::ZeroVector)
		bCheck = true;
	if(InDefaultValue.SpawnRotation != FRotator::ZeroRotator)
		bCheck = true;
	if (InDefaultValue.SpawnScale != FVector::ZeroVector)
		bCheck = true; 
	if (InDefaultValue.SkillCollisionDatas.Num() > 0)
		bCheck = true; 

	if (bCheck == true)
		InternalDatas[InIndex].bChecked = true; 
}

void SSkillCheckBoxes::OnCheckStateChanged(ECheckBoxState InState, int32 InIndex)
{
	InternalDatas[InIndex].bChecked = !InternalDatas[InIndex].bChecked;

	SSkillDetails::OnResfreshByCheckBoxes();
	{
		Utilities->ForceRefresh();
	}
	SSkillDetails::OffResfreshByCheckBoxes();
}

void SSkillCheckBoxes::SetUtilities(TSharedPtr<class IPropertyUtilities> InUtilities)
{
	Utilities = InUtilities;
}
