#include "SkillStyle.h"
#include "Styling/SlateStyleRegistry.h"


const FName FSkillStyle::StyleSetName = "SkillStyle";
TSharedPtr<FSkillStyle> FSkillStyle::Instance = nullptr;

void FSkillStyle::Regist()
{
	Instance = MakeShareable(new FSkillStyle());
}

TSharedPtr<FSkillStyle> FSkillStyle::Get()
{
	ensure(Instance != nullptr);

	return Instance;
}

void FSkillStyle::Unregist()
{
	if (Instance.IsValid())
		Instance.Reset();
}

FSkillStyle::FSkillStyle()
{
	FString path = "";


	StyleSet = MakeShareable(new FSlateStyleSet(StyleSetName));

	path = FPaths::ProjectPluginsDir() / "SkillEditor" / "Resources";
	StyleSet->SetContentRoot(path);

	RegisterIcon("SkillIcon", path / "Skill_Icon.png", FVector2D(40, 40), WeaponIcon);
	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());


	path = FPaths::ProjectPluginsDir() / "SkillEditor" / "Resources" / "White.png";
	DoActionDataBorder = MakeShareable(new FSlateImageBrush(path, FVector2D(16, 16), FLinearColor(0.0f, 0.5f, 1.0f, 0.5f)));
	DamagedDataBorder = MakeShareable(new FSlateImageBrush(path, FVector2D(16, 16), FLinearColor(0.0f, 1.0f, 0.5f, 0.5f)));
}

FSkillStyle::~FSkillStyle()
{
	if (DoActionDataBorder.IsValid())
		DoActionDataBorder.Reset();

	if (DamagedDataBorder.IsValid())
		DamagedDataBorder.Reset();

	if (StyleSet.IsValid() == false)
		return;

	FSlateStyleRegistry::UnRegisterSlateStyle(StyleSetName);
	StyleSet.Reset();
}

void FSkillStyle::RegisterIcon(const FString& InName, const FString& InPath, const FVector2D& InIconSize, FSlateIcon& OutSlateIcon)
{
	FSlateImageBrush* brush = new FSlateImageBrush(InPath, InIconSize);

	FString name = StyleSetName.ToString() + "." + InName;
	StyleSet->Set(FName(name), brush);

	OutSlateIcon = FSlateIcon(StyleSetName, FName(name));
}
