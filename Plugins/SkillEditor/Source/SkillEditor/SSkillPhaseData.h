#pragma once

#include "CoreMinimal.h"
#include "IPropertyTypeCustomization.h"

class SKILLEDITOR_API SSkillPhaseData
	: public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();
	static TSharedPtr<class SSkillCheckBoxes> AddCheckBoxes();
	static void EmptyCheckBoxes();

	void CustomizeHeader(TSharedRef<IPropertyHandle> InPropertyHandle, FDetailWidgetRow& InHeaderRow, IPropertyTypeCustomizationUtils& InCustomizationUtils) override;
	void CustomizeChildren(TSharedRef<IPropertyHandle> InPropertyHandle, IDetailChildrenBuilder& InChildBuilder, IPropertyTypeCustomizationUtils& InCustomizationUtils) override;

private:
	static TArray<TSharedPtr<class SSkillCheckBoxes>> CheckBoxes;

private: 
	void OnPhaseTypeChanged();
	void OnActionDataChanged();
	void OnEffectChanged();

	void RebuildChildren(class IDetailPropertyRow& InRow, TSharedPtr<IPropertyHandle> InHandle);

private:
	bool PhaseTypeChanged = false; 
	bool ActionDataTypeChanged = false;
	bool EffectTypeChanged = false;
	bool SoundChanged = false;
	bool CSChanged = false;
	bool EntityChanged = false;

	bool ActionDataVisible = false;
	bool EffectTypeVisible = false;
	bool SoundVisible = false;
	bool CSVisible = false;
	bool EntityVisible = false;


private:
	TSharedPtr<IPropertyHandle> PhaseTypeHandle; 
	TSharedPtr<IPropertyHandle> ActionDataHandle;
	TSharedPtr<IPropertyHandle> EffectHandle;
	TSharedPtr<IPropertyHandle> EffectLocationHandle;
	TSharedPtr<IPropertyHandle> EffectRotationHandle;
	TSharedPtr<IPropertyHandle> EffectScaleHandle;
	TSharedPtr<IPropertyHandle> SoundHandle;
	TSharedPtr<IPropertyHandle> CameraShakeHandle;
	TSharedPtr<IPropertyHandle> SkillEntityDataHandle;


	TSharedPtr<IPropertyUtilities> PropertyUtilities;
};
