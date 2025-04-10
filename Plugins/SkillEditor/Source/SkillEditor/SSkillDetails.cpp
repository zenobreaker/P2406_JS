#include "SSkillDetails.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "IDetailPropertyRow.h"

#include "Skill/CSkillAsset.h"

bool SSkillDetails::bRefreshByCheckBoxes = false; 

TSharedRef<IDetailCustomization> SSkillDetails::MakeInstance()
{
	return MakeShareable(new SSkillDetails());
}

void SSkillDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
}

void SSkillDetails::AddTestCategory(IDetailLayoutBuilder& DetailBuilder)
{
}

FReply SSkillDetails::OnClicked_Test()
{
	return FReply::Handled();
}
