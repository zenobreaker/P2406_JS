#include "SSkillDetails.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "IDetailPropertyRow.h"

#include "SSkillInfoData.h"
#include "SSkillFlowData.h"
#include "SSkillPhaseData.h"
#include "SSkillCheckBoxes.h"


#include "P2406_JS/Skill/CSkillStructures.h"
#include "Skill/CSkillAsset.h"

bool SSkillDetails::bRefreshByCheckBoxes = false;

TSharedRef<IDetailCustomization> SSkillDetails::MakeInstance()
{
	return MakeShareable(new SSkillDetails());
}

void SSkillDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{

	UClass* type = UCSkillAsset::StaticClass();

	//Type Setting 
	{
		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("TypeSettings", FText::FromString(" - Type Setting"));
		category.AddProperty("WeaponType", type);
	}

	// Class Settings
	{
		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("ClassSettings", FText::FromString(" - Class Settings"));
		category.AddProperty("SkillClass", type);
	}

	// Info Data
	{
		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("SkillInfoData", FText::FromString(" - Info Settings"));

		IDetailPropertyRow& row = category.AddProperty("SkillInfo", type);
		row.ShouldAutoExpand(true);

		if (bRefreshByCheckBoxes == false)
		{
			TSharedPtr<SSkillCheckBoxes> checkBoxes = SSkillInfoData::Create();
			checkBoxes->AddProperties(row.GetPropertyHandle());

			TSharedPtr<IPropertyHandle> handle = row.GetPropertyHandle();

			FSkillInfo data;

			int32 index = 0;
			checkBoxes->CheckDefaultValue(index++, (int32)data.WeaponType);
			checkBoxes->CheckDefaultValue(index++, data.SkillID);
			checkBoxes->CheckDefaultObject(index++, data.SkillIcon);
			checkBoxes->CheckDefaultValue(index++, data.SkillName);
			checkBoxes->CheckDefaultValue(index++, data.CastingTime);
			checkBoxes->CheckDefaultValue(index++, data.CoolDown);
			checkBoxes->CheckDefaultValue(index++, data.Cost);
		}
	}
	
	// Flow 
	{
		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("SkillFlowData", FText::FromString(" - Flow Settings"));
		IDetailPropertyRow& row = category.AddProperty("FlowData", type);
		//row.ShouldAutoExpand();
		
		TSharedPtr<IPropertyHandle> handle = row.GetPropertyHandle();
	}

}

void SSkillDetails::AddTestCategory(IDetailLayoutBuilder& DetailBuilder)
{
}

FReply SSkillDetails::OnClicked_Test()
{
	return FReply::Handled();
}
