#include "WeaponStyle.h"
#include "Styling/SlateStyleRegistry.h"


const FName FWeaponStyle::StyleSetName = "WeaponStyle";
TSharedPtr<FWeaponStyle> FWeaponStyle::Instance = nullptr;

void FWeaponStyle::Regist()
{
	Instance = MakeShareable(new FWeaponStyle()); 
}

TSharedPtr<FWeaponStyle> FWeaponStyle::Get()
{
	ensure(Instance != nullptr); 

	return Instance;
}

void FWeaponStyle::Unregist()
{
	if (Instance.IsValid())
		Instance.Reset();
}

FWeaponStyle::FWeaponStyle()
{
	FString path = "";


	StyleSet = MakeShareable(new FSlateStyleSet(StyleSetName));

	path = FPaths::ProjectPluginsDir() / "WeaponEditor" / "Resources";
	StyleSet->SetContentRoot(path);

	RegisterIcon("WeaponIcon", path / "Weapon_Icon.png", FVector2D(40, 40), WeaponIcon);
	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());


	path = FPaths::ProjectPluginsDir() / "WeaponEditor" / "Resources" / "White.png";
	DoActionDataBorder = MakeShareable(new FSlateImageBrush(path, FVector2D(16, 16), FLinearColor(0.0f, 0.5f, 1.0f, 0.5f)));
	DamagedDataBorder = MakeShareable(new FSlateImageBrush(path, FVector2D(16, 16), FLinearColor(0.0f, 1.0f, 0.5f, 0.5f)));
}

FWeaponStyle::~FWeaponStyle()
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

void FWeaponStyle::RegisterIcon(const FString& InName, const FString& InPath, const FVector2D& InIconSize, FSlateIcon& OutSlateIcon)
{
	FSlateImageBrush* brush = new FSlateImageBrush(InPath, InIconSize); 

	FString name = StyleSetName.ToString() + "." + InName; 
	StyleSet->Set(FName(name), brush); 

	OutSlateIcon = FSlateIcon(StyleSetName, FName(name)); 
}
