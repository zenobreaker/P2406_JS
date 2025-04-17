#include "SWeaponCheckBoxes.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "IPropertyUtilities.h"
#include "Widgets/Layout/SUniformGridPanel.h"

#include "SWeaponDetails.h"
#include "Weapons/CWeaponStructures.h"

void SWeaponCheckBoxes::AddProperties(TSharedPtr<IPropertyHandle> InHandle)
{
	uint32 count = 0;
	InHandle->GetNumChildren(count);

	for (uint32 i = 0; i < count; i++)
	{
		TSharedPtr<IPropertyHandle> child = InHandle->GetChildHandle(i);

		if (child->GetProperty()->GetFName().IsEqual(FName("EffectLocation")) ||
			child->GetProperty()->GetFName().IsEqual(FName("EffectScale")))
			continue;

		InternalDatas.Add(FInternalData(child));
	}
}

bool SWeaponCheckBoxes::CanDraw(TSharedPtr<class IPropertyHandle> InHandle, int32 InCount)
{
	bool b = true;
	b &= InCount > 0;

	int32 index = InHandle->GetIndexInArray();
	b &= index >= 0;
	b &= index < InCount;

	return b;
}

TSharedRef<SWidget> SWeaponCheckBoxes::DrawHeader()
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
					.OnCheckStateChanged(this, &SWeaponCheckBoxes::OnCheckStateChanged, i)
					[
						SNew(SBox)
							.MinDesiredWidth(25.0f)
							.MaxDesiredWidth(100.0f)
							[
								SNew(STextBlock)
									.MinDesiredWidth(25.0f)
									.Text(FText::FromString(InternalDatas[i].Name))
									.ToolTipText(FText::FromString(InternalDatas[i].Name))
									.AutoWrapText(true)
									.TextShapingMethod(ETextShapingMethod::Auto)
									.Clipping(EWidgetClipping::ClipToBounds)
							]
						
					]
			];
	}

	return panel.ToSharedRef();
}

void SWeaponCheckBoxes::DrawProperties(TSharedRef<IPropertyHandle> InPropertyHandle, IDetailChildrenBuilder* InChildBuilder)
{
	for (int32 i = 0; i < InternalDatas.Num(); i++)
	{
		TSharedPtr<IPropertyHandle> child = InPropertyHandle->GetChildHandle(i);
		FName propertyName = child->GetProperty()->GetFName(); 

		if (InternalDatas[i].bChecked == false)
			continue;
		IDetailPropertyRow& row = InChildBuilder->AddProperty(child.ToSharedRef());
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

		if (propertyName.IsEqual("Effect") && bDrawComplete == false)
		{
			
			UObject* fx;
			if (child->GetValue(fx) != FPropertyAccess::Success || fx == nullptr)
				continue; 

			const UStruct* OwnerStruct = child->GetProperty()->GetOwnerStruct();
			if (OwnerStruct && OwnerStruct->GetFName() == TEXT("HitData"))
			{
				TSharedPtr<IPropertyHandle> EffectLocationHandle = InPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FHitData, EffectLocation));
				TSharedPtr<IPropertyHandle> EffectScaleHandle = InPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FHitData, EffectScale));

				if (EffectLocationHandle.IsValid() && EffectScaleHandle.IsValid())
				{
					InChildBuilder->AddProperty(EffectLocationHandle.ToSharedRef());
					InChildBuilder->AddProperty(EffectScaleHandle.ToSharedRef());
					bDrawComplete = true; // 플래그 올려서 다음엔 안 그리게
				}
			}
		}

	}
}

void SWeaponCheckBoxes::CheckDefaultObject(int32 InIndex, UObject* InDefaultValue)
{
	UObject* value = nullptr;
	InternalDatas[InIndex].Handle->GetValue(value);

	if (!!value && InDefaultValue != value)
		InternalDatas[InIndex].bChecked = true;
}

void SWeaponCheckBoxes::CheckDefaultValue(int32 InIndex, float InDefaultValue)
{
	float value = 0.0f;
	InternalDatas[InIndex].Handle->GetValue(value);

	if (InDefaultValue != value)
		InternalDatas[InIndex].bChecked = true;
}

void SWeaponCheckBoxes::CheckDefaultValue(int32 InIndex, bool InDefaultValue)
{
	bool value = false;
	InternalDatas[InIndex].Handle->GetValue(value);

	if (InDefaultValue != value)
		InternalDatas[InIndex].bChecked = true;
}

void SWeaponCheckBoxes::CheckDefaultValue(int32 InIndex, const FVector& InDefaultValue)
{
	FVector value = FVector::ZeroVector;
	InternalDatas[InIndex].Handle->GetValue(value);

	if (InDefaultValue != value)
		InternalDatas[InIndex].bChecked = true;
}

void SWeaponCheckBoxes::OnCheckStateChanged(ECheckBoxState InState, int32 InIndex)
{
	InternalDatas[InIndex].bChecked = !InternalDatas[InIndex].bChecked;

	SWeaponDetails::OnRefreshByCheckBoxes();
	{
		Utilities->ForceRefresh();
	}
	SWeaponDetails::OffRefreshByCheckBoxes();
}

void SWeaponCheckBoxes::SetUtilities(TSharedPtr<class IPropertyUtilities> InUtilities)
{
	Utilities = InUtilities;
}

