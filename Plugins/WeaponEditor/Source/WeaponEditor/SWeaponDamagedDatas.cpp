#include "SWeaponDamagedDatas.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "IPropertyUtilities.h"

#include "WeaponStyle.h"
#include "SWeaponCheckBoxes.h"
#include "Weapons/CWeaponStructures.h"

TArray<TSharedPtr<SWeaponCheckBoxes>> SWeaponDamagedDatas::CheckBoxes;

TSharedRef<IPropertyTypeCustomization> SWeaponDamagedDatas::MakeInstance()
{
	return MakeShareable(new SWeaponDamagedDatas());
}

TSharedPtr<class SWeaponCheckBoxes> SWeaponDamagedDatas::AddCheckBoxes()
{
	TSharedPtr<SWeaponCheckBoxes> checkBoxes = MakeShareable(new SWeaponCheckBoxes());
	int32 index = CheckBoxes.Add(checkBoxes);


	return CheckBoxes[index];
}

void SWeaponDamagedDatas::EmptyCheckBoxes()
{
	for (TSharedPtr<SWeaponCheckBoxes> checkBoxes : CheckBoxes)
	{
		if (checkBoxes.IsValid())
			checkBoxes.Reset();
	}

	CheckBoxes.Empty();
}

void SWeaponDamagedDatas::CustomizeHeader(TSharedRef<IPropertyHandle> InPropertyHandle, FDetailWidgetRow& InHeaderRow, IPropertyTypeCustomizationUtils& InCustomizationUtils)
{
	if (SWeaponCheckBoxes::CanDraw(InPropertyHandle, CheckBoxes.Num()) == false)
	{
		InHeaderRow
			.NameContent()
			[
				InPropertyHandle->CreatePropertyNameWidget()
			]
			.ValueContent()
			[
				InPropertyHandle->CreatePropertyValueWidget()
			];

		return;
	}


	int32 index = InPropertyHandle->GetIndexInArray();
	CheckBoxes[index]->SetUtilities(InCustomizationUtils.GetPropertyUtilities());


	FString name = InPropertyHandle->GetPropertyDisplayName().ToString();
	name = "Hit Data - " + name;


	InHeaderRow
		.NameContent()
		.HAlign(HAlign_Fill)
		[
			SNew(SBorder)
				.BorderImage(FWeaponStyle::Get()->DamagedDataBorder.Get())
				[
					InPropertyHandle->CreatePropertyNameWidget(FText::FromString(name))
				]
		]
		.ValueContent()
		[
			SNew(SBorder)
				.HAlign(HAlign_Fill)
				.BorderImage(FWeaponStyle::Get()->DamagedDataBorder.Get())
				[
					CheckBoxes[index]->DrawHeader()
				]
		];
}

void SWeaponDamagedDatas::CustomizeChildren(TSharedRef<IPropertyHandle> InPropertyHandle, IDetailChildrenBuilder& InChildBuilder, IPropertyTypeCustomizationUtils& InCustomizationUtils)
{
	PropertyUtilities = InCustomizationUtils.GetPropertyUtilities();

	EffectHandle = InPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FHitData, Effect));
	EffectLocationHandle = InPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FHitData, EffectLocation));
	EffectScaleHandle = InPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FHitData, EffectScale));

	if (EffectHandle.IsValid() && EffectHandle->IsValidHandle())
	{
		EffectHandle->SetOnPropertyValueChanged(FSimpleDelegate::CreateSP(this, &SWeaponDamagedDatas::OnEffectChanged));
	}

	if (SWeaponCheckBoxes::CanDraw(InPropertyHandle, CheckBoxes.Num()) == false)
	{
		uint32 count = 0;
		InPropertyHandle->GetNumChildren(count);

		for (uint32 i = 0; i < count; i++)
		{
			TSharedPtr<IPropertyHandle> child = InPropertyHandle->GetChildHandle(i);
			IDetailPropertyRow& row = InChildBuilder.AddProperty(child.ToSharedRef());

			FText value;
			child->GetValueAsDisplayText(value);

			row.CustomWidget()
				.NameContent()
				[
					child->CreatePropertyNameWidget()
				]
				.ValueContent()
				[
					child->CreatePropertyValueWidget()
				];
		}//for(i)

		return;
	}

	int32 index = InPropertyHandle->GetIndexInArray();
	CheckBoxes[index]->DrawProperties(InPropertyHandle, &InChildBuilder);
}

void SWeaponDamagedDatas::OnEffectChanged()
{
	if (PropertyUtilities.IsValid())
		PropertyUtilities->ForceRefresh();
}