#pragma once

#include "CoreMinimal.h"
#include "IPropertyTypeCustomization.h"

class SKILLEDITOR_API SSkillInfoData
	:public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance(); 
	static TSharedPtr<class SSkillCheckBoxes> Create();

	void CustomizeHeader(TSharedRef<IPropertyHandle> InPropertyHandle, FDetailWidgetRow& InHeaderRow, IPropertyTypeCustomizationUtils& InCustomizationUtils) override;
	void CustomizeChildren(TSharedRef<IPropertyHandle> InPropertyHandle, IDetailChildrenBuilder& InChildBuilder, IPropertyTypeCustomizationUtils& InCustomizationUtils) override;

private:
	static TSharedPtr<class SSkillCheckBoxes> CheckBoxes;
};
