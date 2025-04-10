#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"

class SKILLEDITOR_API SSkillDetails
	: public IDetailCustomization
{
public: 
	static TSharedRef<IDetailCustomization> MakeInstance(); 
	void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

public:
	static void OnResfreshChekBoxes() { bRefreshByCheckBoxes = true; }
	static void OffResfreshChekBoxes() { bRefreshByCheckBoxes = false; }

private:
	static bool bRefreshByCheckBoxes; 

private: 
	void AddTestCategory(IDetailLayoutBuilder& DetailBuilder);
	FReply OnClicked_Test();
};
