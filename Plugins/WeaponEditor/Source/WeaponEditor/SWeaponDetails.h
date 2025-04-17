#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"

class WEAPONEDITOR_API SWeaponDetails
	:public IDetailCustomization
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance(); 
	void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

public:
	static void OnRefreshByCheckBoxes() { bRefreshByCheckBoxes = true; }
	static void OffRefreshByCheckBoxes() { bRefreshByCheckBoxes = false; }

private:
	static bool bRefreshByCheckBoxes;

private:
	void AddTestCategory(IDetailLayoutBuilder& DetailBuilder); 
};
