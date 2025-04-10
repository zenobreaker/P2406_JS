#include "SSkillInfoData.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"

#include "SSkillCheckBoxes.h"

TSharedPtr<SSkillCheckBoxes> SSkillInfoData::CheckBoxes = nullptr;

TSharedRef<IPropertyTypeCustomization> SSkillInfoData::MakeInstance()
{
	return MakeShareable(new SSkillInfoData());
}

TSharedPtr<class SSkillCheckBoxes> SSkillInfoData::Create()
{
	if (CheckBoxes.IsValid())
	{
		CheckBoxes.Reset();
		CheckBoxes = nullptr;
	}

	return CheckBoxes = MakeShareable(new SSkillCheckBoxes());
}

void SSkillInfoData::CustomizeHeader(TSharedRef<IPropertyHandle> InPropertyHandle, FDetailWidgetRow& InHeaderRow, IPropertyTypeCustomizationUtils& InCustomizationUtils)
{
	CheckBoxes->SetUtilities(InCustomizationUtils.GetPropertyUtilities());

	InHeaderRow
	.NameContent()
	[
		SNew(STextBlock)
		.Text(FText::FromString("Header"))
	]
	.ValueContent()
	[
		SNew(STextBlock)
		.Text(FText::FromString("Value"))
	];


	//InHeaderRow
	//	.NameContent()
	//	[
	//		InPropertyHandle->CreatePropertyNameWidget()
	//	];
	//	.ValueContent()
	//	[
	//		//CheckBoxes->DrawHeader()
	//		SNew(STextBlock).Text("dd")
	//	];
}

void SSkillInfoData::CustomizeChildren(TSharedRef<IPropertyHandle> InPropertyHandle, IDetailChildrenBuilder& InChildBuilder, IPropertyTypeCustomizationUtils& InCustomizationUtils)
{
	CheckBoxes->DrawProperties(InPropertyHandle, &InChildBuilder);
}
