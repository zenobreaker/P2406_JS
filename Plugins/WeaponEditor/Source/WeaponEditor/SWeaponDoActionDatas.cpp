#include "SWeaponDoActionDatas.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"

#include "WeaponStyle.h"
#include "SWeaponCheckBoxes.h"

TArray<TSharedPtr<SWeaponCheckBoxes>> SWeaponDoActionDatas::CheckBoxes;

TSharedRef<IPropertyTypeCustomization> SWeaponDoActionDatas::MakeInstance()
{
    return MakeShareable(new SWeaponDoActionDatas());
}

TSharedPtr<class SWeaponCheckBoxes> SWeaponDoActionDatas::AddCheckBoxes()
{
	TSharedPtr<SWeaponCheckBoxes> checkBoxes = MakeShareable(new SWeaponCheckBoxes());
	int32 index = CheckBoxes.Add(checkBoxes);

	return CheckBoxes[index];
}

void SWeaponDoActionDatas::EmptyCheckBoxes()
{
	for (TSharedPtr<SWeaponCheckBoxes> checkBoxes : CheckBoxes)
	{
		if (checkBoxes.IsValid())
			checkBoxes.Reset();
	}

	CheckBoxes.Empty();
}

void SWeaponDoActionDatas::CustomizeHeader(TSharedRef<IPropertyHandle> InPropertyHandle, FDetailWidgetRow& InHeaderRow, IPropertyTypeCustomizationUtils& InCustomizationUtils)
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
	name = "DoActionData - " + name;


	InHeaderRow
		.NameContent()
		.HAlign(HAlign_Fill)
		[
			SNew(SBorder)
				.BorderImage(FWeaponStyle::Get()->DoActionDataBorder.Get())
				[
					InPropertyHandle->CreatePropertyNameWidget(FText::FromString(name))
				]
		]
		.ValueContent()
		[
			SNew(SBorder)
				.HAlign(HAlign_Fill)
				.BorderImage(FWeaponStyle::Get()->DoActionDataBorder.Get())
				[
					CheckBoxes[index]->DrawHeader()
				]
		];
}

void SWeaponDoActionDatas::CustomizeChildren(TSharedRef<IPropertyHandle> InPropertyHandle, IDetailChildrenBuilder& InChildBuilder, IPropertyTypeCustomizationUtils& InCustomizationUtils)
{
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
