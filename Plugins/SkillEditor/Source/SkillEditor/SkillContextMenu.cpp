#include "SkillContextMenu.h"
#include "SkillAssetEditor.h"

#include "Skill/CSkillAsset.h"


FSkillContextMenu::FSkillContextMenu(EAssetTypeCategories::Type InType)
{
	Category = InType;
}

FText FSkillContextMenu::GetName() const
{
	return FText::FromString("SkillAsset");;
}

UClass* FSkillContextMenu::GetSupportedClass() const
{
	return UCSkillAsset::StaticClass();
}

FColor FSkillContextMenu::GetTypeColor() const
{
	return FColor::Yellow;
}

uint32 FSkillContextMenu::GetCategories()
{
	return Category;
}

const FSlateBrush* FSkillContextMenu::GetThumbnailBrush(const FAssetData& InAssetData, const FName InClassName) const
{
	if (InClassName == TEXT("CSkillAsset"))
	{
		UCSkillAsset* skillAsset = Cast<UCSkillAsset>(InAssetData.GetAsset());
		if (skillAsset && skillAsset->SkillInfo.SkillIcon)
		{
			FSlateBrush* brush = new FSlateBrush();
			brush->SetResourceObject(skillAsset->SkillInfo.SkillIcon);
			return brush;
		}
	}
	
	return nullptr;
}

void FSkillContextMenu::OpenAssetEditor(const TArray<UObject*>& InObjects, const EAssetTypeActivationOpenedMethod OpenedMethod, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	//GLog->Logf(L"%d\n", OpenedMethod);

	//for (UObject* obj : InObjects)
	//{
	//	if (!!obj)
	//		GLog->Log(obj->GetName());
	//}

	if (InObjects.Num() < 1)
		return;

	FSkillAssetEditor::OpenWindow(InObjects[0]->GetName());
}

