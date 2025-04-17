#pragma once

#include "CoreMinimal.h"
#include "Skill/CSkillStructures.h"
#include "IPropertyTypeCustomization.h"

class SKILLEDITOR_API SSkillCollisionDatas
	: public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

	void CustomizeHeader(TSharedRef<IPropertyHandle> InPropertyHandle, FDetailWidgetRow& InHeaderRow, IPropertyTypeCustomizationUtils& InCustomizationUtils) override;
	void CustomizeChildren(TSharedRef<IPropertyHandle> InPropertyHandle, IDetailChildrenBuilder& InChildBuilder, IPropertyTypeCustomizationUtils& InCustomizationUtils) override;

	void GenerateArrayElement(TSharedRef<IPropertyHandle> ElementHandle, int32 Index, IDetailChildrenBuilder& ChildrenBuilder);

private:
	void OnCollisionClassChanged(); 
	void OnCollisionTypeChanged(); 

	void RebuildChildren(class IDetailPropertyRow& InRow, TSharedPtr<IPropertyHandle> InHandle);

public:
	static void OnResfreshByCheckBoxes() { bRefreshByCheckBoxes = true; }
	static void OffResfreshByCheckBoxes() { bRefreshByCheckBoxes = false; }

private:
	static bool bRefreshByCheckBoxes;

private: 
	bool bCollsionClassEnable = false; 
	bool bTypeDrawable = false; 

	ESkillCollisionType Type = ESkillCollisionType::Max;

private:
	TSharedPtr<IPropertyHandle> SkillCollisionClassHandle; 

	TSharedPtr<IPropertyHandle> CollsiionTypeHandle;
	TSharedPtr<IPropertyHandle> BoxExtentHandle;
	TSharedPtr<IPropertyHandle> CapsuleRadiusHandle;
	TSharedPtr<IPropertyHandle> CapsuleHalfHandle;

	

	TSharedPtr<IPropertyUtilities> PropertyUtilities;
};
