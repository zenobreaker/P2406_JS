#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"


struct FSkillPhaseData;
struct FSkillEntityData;

class SKILLEDITOR_API SSkillDetails
	: public IDetailCustomization
{
public: 
	static TSharedRef<IDetailCustomization> MakeInstance(); 
	void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

public:
	static void OnResfreshByCheckBoxes() { bRefreshByCheckBoxes = true; }
	static void OffResfreshByCheckBoxes() { bRefreshByCheckBoxes = false; }

private:
	static bool bRefreshByCheckBoxes; 

private: 
	void AddTestCategory(IDetailLayoutBuilder& DetailBuilder);
	FReply OnClicked_Test();
};
