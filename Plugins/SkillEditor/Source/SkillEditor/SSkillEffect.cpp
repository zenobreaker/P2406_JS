#include "SSkillEffect.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "IPropertyUtilities.h"

#include "SSkillCheckBoxes.h"

#include "Skill/CSkillAsset.h"

TSharedPtr<SSkillCheckBoxes> SSkillEffect::CheckBoxes = nullptr;

TSharedRef<IPropertyTypeCustomization> SSkillEffect::MakeInstance()
{
	return MakeShareable(new SSkillEffect());
}

TSharedPtr<class SSkillCheckBoxes> SSkillEffect::CreateCheckBox()
{
	if (CheckBoxes.IsValid())
	{
		CheckBoxes.Reset();
		CheckBoxes = nullptr;
	}

	return CheckBoxes = MakeShareable(new SSkillCheckBoxes());
}


void SSkillEffect::CustomizeHeader(TSharedRef<IPropertyHandle> InPropertyHandle, FDetailWidgetRow& InHeaderRow, IPropertyTypeCustomizationUtils& InCustomizationUtils)
{
	if (CheckBoxes.IsValid())
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
			];

		return; 
	}

	//InHeaderRow
	//	.NameContent()
	//	[
	//		InPropertyHandle->CreatePropertyNameWidget()
	//	]
	//	.ValueContent()
	//	[
	//		InPropertyHandle->CreatePropertyValueWidget()
	//	];
}

void SSkillEffect::CustomizeChildren(TSharedRef<IPropertyHandle> InPropertyHandle, IDetailChildrenBuilder& InChildBuilder, IPropertyTypeCustomizationUtils& InCustomizationUtils)
{
	//PropertyUtilities = InCustomizationUtils.GetPropertyUtilities();
	//
	//EffectHandle = InPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSkillEffectInfo, SkillEffectAsset));
	//
	//if (EffectHandle.IsValid() && EffectHandle->IsValidHandle())
	//{
	//	EffectHandle->SetOnPropertyValueChanged(FSimpleDelegate::CreateSP(this, &SSkillEffect::OnEffectChanged));

	//	UObject* fx;
	//	if (EffectHandle->GetValue(fx) == FPropertyAccess::Success && fx != nullptr)
	//		bEffectValueChanged = true;
	//	else
	//		bEffectValueChanged = false;
	//}

	//CheckBoxes->DrawProperties(InPropertyHandle, &InChildBuilder);
	//InChildBuilder.AddProperty(EffectHandle.ToSharedRef());
}

void SSkillEffect::OnEffectChanged()
{
	if (EffectHandle.IsValid() == false || PropertyUtilities.IsValid() == false)
	{
		return;
	}

	UObject* fx;
	if (EffectHandle->GetValue(fx) == FPropertyAccess::Success && fx != nullptr)
	{
		bEffectValueChanged = true;
	}
	else
		bEffectValueChanged = false;

	PropertyUtilities->ForceRefresh();
}
