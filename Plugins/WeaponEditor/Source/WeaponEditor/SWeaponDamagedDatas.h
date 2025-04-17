#pragma once

#include "CoreMinimal.h"
#include "IPropertyTypeCustomization.h"

class WEAPONEDITOR_API SWeaponDamagedDatas
	: public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();
	static TSharedPtr<class SWeaponCheckBoxes> AddCheckBoxes();
	static void EmptyCheckBoxes();

	void CustomizeHeader(TSharedRef<IPropertyHandle> InPropertyHandle, FDetailWidgetRow& InHeaderRow, IPropertyTypeCustomizationUtils& InCustomizationUtils) override;
	void CustomizeChildren(TSharedRef<IPropertyHandle> InPropertyHandle, IDetailChildrenBuilder& InChildBuilder, IPropertyTypeCustomizationUtils& InCustomizationUtils) override;

private:
	static TArray<TSharedPtr<class SWeaponCheckBoxes>> CheckBoxes;

private:
	void OnEffectChanged();

private:
	TSharedPtr<IPropertyHandle> EffectHandle;
	TSharedPtr<IPropertyHandle> EffectLocationHandle;
	TSharedPtr<IPropertyHandle> EffectScaleHandle;

	TSharedPtr<IPropertyUtilities> PropertyUtilities;
};
