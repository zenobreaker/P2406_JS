#include "SSkillCollisionDatas.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include <DetailLayoutBuilder.h>
#include "IPropertyUtilities.h"

#include "SSkillEffect.h"

#include "Skill/CSkillAsset.h"
#include <SSkillCheckBoxes.h>

bool SSkillCollisionDatas::bRefreshByCheckBoxes = false;

TSharedRef<IPropertyTypeCustomization> SSkillCollisionDatas::MakeInstance()
{
	return MakeShareable(new SSkillCollisionDatas());
}

void SSkillCollisionDatas::CustomizeHeader(TSharedRef<IPropertyHandle> InPropertyHandle, FDetailWidgetRow& InHeaderRow, IPropertyTypeCustomizationUtils& InCustomizationUtils)
{
	InHeaderRow.NameContent()
		[
			InPropertyHandle->CreatePropertyNameWidget()
		]
		.ValueContent()
		[
			InPropertyHandle->CreatePropertyValueWidget()
		];
}

void SSkillCollisionDatas::CustomizeChildren(TSharedRef<IPropertyHandle> InPropertyHandle,
	IDetailChildrenBuilder& InChildBuilder,
	IPropertyTypeCustomizationUtils& InCustomizationUtils)
{
	PropertyUtilities = InCustomizationUtils.GetPropertyUtilities();

	SkillCollisionClassHandle = InPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSkillCollisionData, SkillCollisionClass));
	CollsiionTypeHandle = InPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSkillCollisionData, CollisionType));
	BoxExtentHandle = InPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSkillCollisionData, BoxExtent));
	CapsuleRadiusHandle = InPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSkillCollisionData, CapsuleRadius));
	CapsuleHalfHandle = InPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSkillCollisionData, CapsuleHalfHeight));


	if (SkillCollisionClassHandle.IsValid() && SkillCollisionClassHandle->IsValidHandle())
	{
		InChildBuilder.AddProperty(SkillCollisionClassHandle.ToSharedRef());

		SkillCollisionClassHandle->SetOnPropertyValueChanged(FSimpleDelegate::CreateSP(this, &SSkillCollisionDatas::OnCollisionClassChanged));

		UObject* collisionClass;
		if (SkillCollisionClassHandle->GetValue(collisionClass) == FPropertyAccess::Success &&
			collisionClass != nullptr)
			bCollsionClassEnable = true;
		else
			bCollsionClassEnable = false; 
	}


	if (CollsiionTypeHandle.IsValid() && CollsiionTypeHandle->IsValidHandle())
	{

		CollsiionTypeHandle->SetOnPropertyValueChanged(FSimpleDelegate::CreateSP(this, &SSkillCollisionDatas::OnCollisionTypeChanged));

		uint8 v = (uint8)ESkillCollisionType::Max;
		if (CollsiionTypeHandle->GetValue(v) == FPropertyAccess::Success)
			Type = (ESkillCollisionType)v;
	}


	if (bCollsionClassEnable == false)
		return;

	uint32 count = 0;
	InPropertyHandle->GetNumChildren(count);

	for (uint32 i = 0; i < count; i++)
	{
		TSharedPtr<IPropertyHandle> child = InPropertyHandle->GetChildHandle(i);

		if (child->GetProperty()->GetName().Equals("BoxExtent") && Type != ESkillCollisionType::Box)
		{
			continue;
		}

		if (child->GetProperty()->GetName().Equals("CapsuleRadius") &&
			(Type == ESkillCollisionType::Box || Type == ESkillCollisionType::Max))
		{
			continue;
		}

		if (child->GetProperty()->GetName().Equals("CapsuleHalfHeight") &&
			(Type != ESkillCollisionType::Capsule))
		{
			continue;
		}

		if (child->GetProperty()->GetName().Equals("SkillCollisionClass"))
			continue;

		IDetailPropertyRow& row = InChildBuilder.AddProperty(child.ToSharedRef());

	/*	if (child->GetProperty()->GetName().Equals("SkillEffect") && bRefreshByCheckBoxes == false)
		{
			TSharedPtr<SSkillCheckBoxes> checkboxes = SSkillEffect::CreateCheckBox(); 
			checkboxes->AddProperties(row.GetPropertyHandle());

			FSkillEffectInfo info; 
			uint32 index = 0;
			checkboxes->CheckDefaultObject(index++, info.SkillEffectAsset);
			checkboxes->CheckDefaultValue(index++, info.EffectLocation);
			checkboxes->CheckDefaultValue(index++, info.EffectRotation);
			checkboxes->CheckDefaultValue(index++, info.EffectScale);
		}*/
	}
}


void SSkillCollisionDatas::GenerateArrayElement(TSharedRef<IPropertyHandle> ElementHandle, int32 Index, IDetailChildrenBuilder& ChildrenBuilder)
{
	ChildrenBuilder.AddProperty(ElementHandle);
}

void SSkillCollisionDatas::OnCollisionClassChanged()
{
	if (SkillCollisionClassHandle.IsValid() == false || PropertyUtilities.IsValid() == false)
		return;

	UObject* collsionClass;
	if (SkillCollisionClassHandle->GetValue(collsionClass) == FPropertyAccess::Success && collsionClass != nullptr)
		bCollsionClassEnable = true;
	else
		bCollsionClassEnable = false;

	PropertyUtilities->ForceRefresh();
}

void SSkillCollisionDatas::OnCollisionTypeChanged()
{
	if (CollsiionTypeHandle.IsValid() == false || PropertyUtilities.IsValid() == false)
		return;

	uint8 v = (uint8)ESkillCollisionType::Max;
	if (CollsiionTypeHandle->GetValue(v) != FPropertyAccess::Success)
		return;

	Type = (ESkillCollisionType)v;

	PropertyUtilities->ForceRefresh();
}

void SSkillCollisionDatas::RebuildChildren(IDetailPropertyRow& InRow, TSharedPtr<IPropertyHandle> InHandle)
{
	InRow.CustomWidget()
		.NameContent()
		[
			InHandle->CreatePropertyNameWidget()
		]
		.ValueContent()
		[
			InHandle->CreatePropertyValueWidget()
		];
}
