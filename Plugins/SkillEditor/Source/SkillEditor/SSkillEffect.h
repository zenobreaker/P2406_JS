#pragma once

#include "CoreMinimal.h"
#include "IPropertyTypeCustomization.h"

class SKILLEDITOR_API SSkillEffect
	: public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();
	static TSharedPtr<class SSkillCheckBoxes> CreateCheckBox();


	void CustomizeHeader(TSharedRef<IPropertyHandle> InPropertyHandle, FDetailWidgetRow& InHeaderRow, IPropertyTypeCustomizationUtils& InCustomizationUtils) override;
	void CustomizeChildren(TSharedRef<IPropertyHandle> InPropertyHandle, IDetailChildrenBuilder& InChildBuilder, IPropertyTypeCustomizationUtils& InCustomizationUtils) override;

private:
	static TSharedPtr<class SSkillCheckBoxes> CheckBoxes;

private:
	void OnEffectChanged(); 

private:
	bool bEffectValueChanged = false; 

private:
	TSharedPtr<IPropertyHandle> EffectHandle;
	TSharedPtr<IPropertyHandle> EffectLocationHandle;
	TSharedPtr<IPropertyHandle> EffectRotationHandle;
	TSharedPtr<IPropertyHandle> EffectScaleHandle;

	TSharedPtr<IPropertyUtilities> PropertyUtilities;
};
