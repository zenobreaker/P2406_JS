#include "SWeaponEquipmentData.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"

#include "SWeaponCheckBoxes.h"

TSharedPtr<SWeaponCheckBoxes> SWeaponEquipmentData::CheckBoxes = nullptr;

TSharedRef<IPropertyTypeCustomization> SWeaponEquipmentData::MakeInstance()
{
	return MakeShareable(new SWeaponEquipmentData());
}

TSharedPtr<SWeaponCheckBoxes> SWeaponEquipmentData::Create()
{
	if (CheckBoxes.IsValid())
	{
		CheckBoxes.Reset(); 
		CheckBoxes = nullptr;
	}

	return CheckBoxes = MakeShareable(new SWeaponCheckBoxes());
}

void SWeaponEquipmentData::CustomizeHeader(TSharedRef<IPropertyHandle> InPropertyHandle, FDetailWidgetRow& InHeaderRow, IPropertyTypeCustomizationUtils& InCustomizationUtils)
{
	CheckBoxes->SetUtilities(InCustomizationUtils.GetPropertyUtilities());

	InHeaderRow
		.NameContent()
		[
			InPropertyHandle->CreatePropertyNameWidget()
		]
		.ValueContent()
		[
			CheckBoxes->DrawHeader()
				//InPropertyHandle->CreatePropertyValueWidget()
		];
}

void SWeaponEquipmentData::CustomizeChildren(TSharedRef<IPropertyHandle> InPropertyHandle, IDetailChildrenBuilder& InChildBuilder, IPropertyTypeCustomizationUtils& InCustomizationUtils)
{
	CheckBoxes->DrawProperties(InPropertyHandle, &InChildBuilder);
}
