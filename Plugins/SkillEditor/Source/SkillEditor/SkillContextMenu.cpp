#include "SkillContextMenu.h"
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

void FSkillContextMenu::OpenAssetEditor(const TArray<UObject*>& InObjects, const EAssetTypeActivationOpenedMethod OpenedMethod, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	GLog->Logf(L"%d\n", OpenedMethod);

	//for (UObject* obj : InObjects)
	//{
	//	if (!!obj)
	//		GLog->Log(obj->GetName());
	//}

	if (InObjects.Num() < 1)
		return;

	//FWeaponAssetEditor::OpenWindow(InObjects[0]->GetName());
}

