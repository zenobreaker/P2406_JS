#include "SWeaponDetails.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "IDetailPropertyRow.h"

#include "SWeaponDamagedDatas.h"
#include "SWeaponDoActionDatas.h"
#include "SWeaponEquipmentData.h"
#include "SWeaponCheckBoxes.h"

#include "Weapons/CWeaponStructures.h"
#include "Weapons/CWeaponAsset.h"

bool SWeaponDetails::bRefreshByCheckBoxes = false;

TSharedRef<IDetailCustomization> SWeaponDetails::MakeInstance()
{
	return MakeShareable(new SWeaponDetails());
}

void SWeaponDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	UClass* type = UCWeaponAsset::StaticClass(); 


	//Type Setting
	{
		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("TypeSetting", FText::FromString(" - Type Setting"));
		category.AddProperty("WeaponType", type);
	}

	//Class Settings
	{
		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("ClassSettings", FText::FromString(" - Class Settings"));
		category.AddProperty("AttachmentClass", type);
		category.AddProperty("EquipmentClass", type);
		category.AddProperty("DoActionClass", type);
		category.AddProperty("SubActionClass", type);
		category.AddProperty("ExecuteActionClass", type);
	}

	//Equipment Data
	{
		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("EquipmentData", FText::FromString(" - Equipment Data"));

		IDetailPropertyRow& row = category.AddProperty("EquipmentData", type);
		row.ShouldAutoExpand(true);
	

		if (bRefreshByCheckBoxes == false)
		{
			TSharedPtr<SWeaponCheckBoxes> checkBoxes = SWeaponEquipmentData::Create();
			checkBoxes->AddProperties(row.GetPropertyHandle());


			FEquipmentData data;

			int32 index = 0;
			checkBoxes->CheckDefaultObject(index++, data.Montage);
			checkBoxes->CheckDefaultValue(index++, data.PlayRate);
			checkBoxes->CheckDefaultValue(index++, data.bCanMove);
		}
	}

	//DoActionDatas
	{
		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("DoActionDatas", FText::FromString(" - DoAction Datas"));

		IDetailPropertyRow& row = category.AddProperty("DoActionDatas", type);
		row.ShouldAutoExpand(true);

		if (bRefreshByCheckBoxes == false)
		{
			SWeaponDoActionDatas::EmptyCheckBoxes();


			uint32 count = 0;
			row.GetPropertyHandle()->GetNumChildren(count);

			FDoActionData data;
			for (uint32 i = 0; i < count; i++)
			{
				TSharedPtr<IPropertyHandle> child = row.GetPropertyHandle()->GetChildHandle(i);

				TSharedPtr<SWeaponCheckBoxes> checkBoxes = SWeaponDoActionDatas::AddCheckBoxes();
				checkBoxes->AddProperties(child);

				int32 index = 0;
				checkBoxes->CheckDefaultObject(index++, data.Montage);
				checkBoxes->CheckDefaultValue(index++, data.PlayRate);
				checkBoxes->CheckDefaultValue(index++, data.bCanMove);
				checkBoxes->CheckDefaultValue(index++, data.bFixedCamera);
				//checkBoxes->CheckDefaultObject(index++, data.GhostTrailClass);
			}
		}//if (bRefreshByCheckBoxes)
	}

	//DamagedDatas
	{
		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("HitDatas", FText::FromString(" - Hit Datas"));

		IDetailPropertyRow& row = category.AddProperty("HitDatas", type);
		row.ShouldAutoExpand(true);

		if (bRefreshByCheckBoxes == false)
		{
			SWeaponDamagedDatas::EmptyCheckBoxes();


			uint32 count = 0;
			row.GetPropertyHandle()->GetNumChildren(count);

			FHitData data;
			for (uint32 i = 0; i < count; i++)
			{
				TSharedPtr<IPropertyHandle> child = row.GetPropertyHandle()->GetChildHandle(i);
				
				TSharedPtr<SWeaponCheckBoxes> checkBoxes = SWeaponDamagedDatas::AddCheckBoxes();
				checkBoxes->AddProperties(child);

				int32 index = 0;
				checkBoxes->CheckDefaultObject(index++, data.Montage);
				checkBoxes->CheckDefaultValue(index++, data.PlayRate);
				checkBoxes->CheckDefaultValue(index++, data.Power);
				checkBoxes->CheckDefaultValue(index++, data.Launch);
				checkBoxes->CheckDefaultValue(index++, data.Airial);
				checkBoxes->CheckDefaultValue(index++, data.StopTime);
				checkBoxes->CheckDefaultValue(index++, data.bDown);
				checkBoxes->CheckDefaultObject(index++, data.Sound);
				checkBoxes->CheckDefaultObject(index++, data.Effect);
				checkBoxes->CheckDefaultObject(index++, data.CameraShake);
			}
		}//if (bRefreshByCheckBoxes)
	}

	// Jump Action Settings
	{
		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("JumpActionSetting", FText::FromString(" - Jump Action Settings"));
		IDetailPropertyRow& row = category.AddProperty("JumpAcitonAsset", type);
	}

	// Skill Settings
	{
		IDetailCategoryBuilder& category = DetailBuilder.EditCategory("SkillSetting", FText::FromString(" - Skill Settings"));
		IDetailPropertyRow& row = category.AddProperty("ActiveSkillAssets", type);
	}
}

void SWeaponDetails::AddTestCategory(IDetailLayoutBuilder& DetailBuilder)
{
}
