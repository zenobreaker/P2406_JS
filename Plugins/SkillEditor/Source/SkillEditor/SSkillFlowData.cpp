#include "SSkillFlowData.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include <DetailLayoutBuilder.h>

#include "SSkillCheckBoxes.h"
#include "Skill/CSkillAsset.h"

TSharedRef<IPropertyTypeCustomization> SSkillFlowData::MakeInstance()
{
	return MakeShareable(new SSkillFlowData());
}

void SSkillFlowData::CustomizeHeader(TSharedRef<IPropertyHandle> InPropertyHandle, FDetailWidgetRow& InHeaderRow, IPropertyTypeCustomizationUtils& InCustomizationUtils)
{
	// 배열 요소 수
	uint32 numElemnts = 0;
	TSharedPtr<IPropertyHandle> PhaseDataHandle = InPropertyHandle->GetChildHandle("PhaseDatas");
	if (PhaseDataHandle.IsValid() && PhaseDataHandle->AsArray().IsValid())
	{
		PhaseDataHandle->GetNumChildren(numElemnts);
	}
	InHeaderRow
		.NameContent()
		//.HAlign(HAlign_Fill)
		[

			/*SNew(SBorder)
				.BorderImage(FSkillStyle::Get()->DoActionDataBorder.Get())
				[*/
					InPropertyHandle->CreatePropertyNameWidget()
				//]
		]
		.ValueContent()
		[
			SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					InPropertyHandle->CreatePropertyValueWidget()
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(5.0f, 0.0f)
				[
					SNew(STextBlock)
						.Text(FText::Format(NSLOCTEXT("SkillFlow", "PhaseCount", "Count: {0}"), FText::AsNumber(numElemnts)))
						.Font(IDetailLayoutBuilder::GetDetailFont())
				]
		];
}

void SSkillFlowData::CustomizeChildren(TSharedRef<IPropertyHandle> InPropertyHandle, 
	IDetailChildrenBuilder& InChildBuilder, 
	IPropertyTypeCustomizationUtils& InCustomizationUtils)
{
	if (InPropertyHandle->IsValidHandle())
	{
		TSharedPtr<IPropertyHandle> PhaseDataHandle = InPropertyHandle->GetChildHandle("PhaseDatas");
		InChildBuilder.AddProperty(PhaseDataHandle.ToSharedRef());
	}
}

void SSkillFlowData::GenerateArrayElement(TSharedRef<IPropertyHandle> ElementHandle, int32 Index, IDetailChildrenBuilder& ChildrenBuilder)
{
	ChildrenBuilder.AddProperty(ElementHandle);
}