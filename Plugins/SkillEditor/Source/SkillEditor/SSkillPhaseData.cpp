#include "SSkillPhaseData.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "IPropertyUtilities.h"

#include "SSkillCheckBoxes.h"
#include "Skill/CSkillAsset.h"

TArray<TSharedPtr<SSkillCheckBoxes>> SSkillPhaseData::CheckBoxes;

TSharedRef<IPropertyTypeCustomization> SSkillPhaseData::MakeInstance()
{
	return MakeShareable(new SSkillPhaseData());
}

TSharedPtr<class SSkillCheckBoxes> SSkillPhaseData::AddCheckBoxes()
{
	TSharedPtr<SSkillCheckBoxes> checkBoxes = MakeShareable(new SSkillCheckBoxes());
	int32 index = CheckBoxes.Add(checkBoxes);

	return CheckBoxes[index];
}

void SSkillPhaseData::EmptyCheckBoxes()
{
	for (TSharedPtr<SSkillCheckBoxes> checkBoxes : CheckBoxes)
	{
		if (checkBoxes.IsValid())
			checkBoxes.Reset();
	}

	CheckBoxes.Empty();
}

void SSkillPhaseData::CustomizeHeader(TSharedRef<IPropertyHandle> InPropertyHandle, FDetailWidgetRow& InHeaderRow, IPropertyTypeCustomizationUtils& InCustomizationUtils)
{

	FString name = InPropertyHandle->GetPropertyDisplayName().ToString();
	name = "Phase Data - " + name;

	InHeaderRow
		.NameContent()
		[
			InPropertyHandle->CreatePropertyNameWidget(FText::FromString(name))
		]
		.ValueContent()
		[
			InPropertyHandle->CreatePropertyValueWidget()
		];
}

void SSkillPhaseData::CustomizeChildren(TSharedRef<IPropertyHandle> InPropertyHandle, IDetailChildrenBuilder& InChildBuilder, IPropertyTypeCustomizationUtils& InCustomizationUtils)
{
	PropertyUtilities = InCustomizationUtils.GetPropertyUtilities();

	PhaseTypeHandle = InPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSkillPhaseData, PhaseType));
	ActionDataHandle = InPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSkillPhaseData, ActionData));
	EffectHandle = InPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSkillPhaseData, Effect));
	EffectLocationHandle = InPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSkillPhaseData, EffectLocation));
	EffectRotationHandle = InPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSkillPhaseData, EffectRotation));
	EffectScaleHandle = InPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSkillPhaseData, EffectScale));
	SoundHandle = InPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSkillPhaseData, Sound));
	CameraShakeHandle = InPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSkillPhaseData, CameraShake));
	SkillEntityDataHandle = InPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSkillPhaseData, SkillEntityData));


	if (PhaseTypeHandle.IsValid() && PhaseTypeHandle->IsValidHandle())
	{
		InChildBuilder.AddProperty(PhaseTypeHandle.ToSharedRef());

		PhaseTypeHandle->SetOnPropertyValueChanged(FSimpleDelegate::CreateSP(this, &SSkillPhaseData::OnPhaseTypeChanged));
	}

	if (EffectHandle.IsValid() && EffectHandle->IsValidHandle())
	{
		EffectHandle->SetOnPropertyValueChanged(FSimpleDelegate::CreateSP(this, &SSkillPhaseData::OnEffectChanged));
	}


	//OnPhaseTypeChanged();
	//OnEffectChanged();

	uint32 count = 0;
	InPropertyHandle->GetNumChildren(count);

	uint8 PhaseTypeValue = (uint8)ESkillPhase::Max;
	if (PhaseTypeHandle->GetValue(PhaseTypeValue) == FPropertyAccess::Success && PhaseTypeValue != (uint8)ESkillPhase::Max)
	{
		
		RebuildChildren(InChildBuilder.AddProperty(ActionDataHandle.ToSharedRef()), ActionDataHandle);
		RebuildChildren(InChildBuilder.AddProperty(EffectHandle.ToSharedRef()), EffectHandle);
		
		
		UObject* fx;
		if (EffectHandle->GetValue(fx) == FPropertyAccess::Success && fx != nullptr)
		{
			InChildBuilder.AddProperty(EffectLocationHandle.ToSharedRef());
			InChildBuilder.AddProperty(EffectRotationHandle.ToSharedRef());
			InChildBuilder.AddProperty(EffectScaleHandle.ToSharedRef());
		}

		RebuildChildren(InChildBuilder.AddProperty(SoundHandle.ToSharedRef()), SoundHandle);
		RebuildChildren(InChildBuilder.AddProperty(CameraShakeHandle.ToSharedRef()), CameraShakeHandle);
		InChildBuilder.AddProperty(SkillEntityDataHandle.ToSharedRef());
	}

	

	//for (uint32 i = 0; i < count; i++)
	//{
	//	TSharedPtr<IPropertyHandle> child = InPropertyHandle->GetChildHandle(i);
	//	IDetailPropertyRow& row = InChildBuilder.AddProperty(child.ToSharedRef());

	//	row.CustomWidget()
	//		.NameContent()
	//		[
	//			child->CreatePropertyNameWidget()
	//		]
	//		.ValueContent()
	//		[
	//			child->CreatePropertyValueWidget()
	//		];
	//}
}

void SSkillPhaseData::OnPhaseTypeChanged()
{
	if (!PhaseTypeHandle.IsValid() || !PropertyUtilities.IsValid())
	{
		return;
	}


	uint8 PhaseTypeValue = (uint8)ESkillPhase::Max;
	if (PhaseTypeHandle->GetValue(PhaseTypeValue) != FPropertyAccess::Success)
	{
		return;
	}


	// PhaseType이 Max가 아니면 다른 프로퍼티들이 보이도록 설정
	if (PhaseTypeValue != (uint8)ESkillPhase::Max)
	{
		PhaseTypeChanged = true; 

		ActionDataVisible = true;
		EffectTypeVisible = true;
		SoundVisible = true;
		CSVisible = true;
		EntityVisible = true;
		PropertyUtilities->ForceRefresh();
	}
	else
	{
		PhaseTypeChanged = false;

		ActionDataVisible = false;
		EffectTypeVisible = false;
		SoundVisible = false;
		CSVisible = false;
		EntityVisible = false;

		PropertyUtilities->ForceRefresh();
	}
}

void SSkillPhaseData::OnActionDataChanged()
{
}

void SSkillPhaseData::OnEffectChanged()
{
	if (EffectHandle.IsValid() == false || PropertyUtilities.IsValid() ==false)
	{
		return; 
	}

	UObject* fx;
	if (EffectHandle->GetValue(fx) == FPropertyAccess::Success && fx != nullptr)
	{
		EffectTypeChanged = true;
	}
	else
		EffectTypeChanged = false; 

	PropertyUtilities->ForceRefresh();
}

void SSkillPhaseData::RebuildChildren(IDetailPropertyRow& InRow, TSharedPtr<IPropertyHandle> InHandle)
{
	if (InHandle == ActionDataHandle)
		return; 

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
