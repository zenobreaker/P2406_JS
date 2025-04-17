#pragma once

#include "CoreMinimal.h"

class WEAPONEDITOR_API FWeaponStyle
{
public:
	static void Regist(); 
	static TSharedPtr<FWeaponStyle> Get(); 
	static void Unregist();
	
private:
	static TSharedPtr<FWeaponStyle> Instance; 

public:
	FWeaponStyle();
	~FWeaponStyle();

private:
	void RegisterIcon(const FString& InName, const FString& InPath, const FVector2D& InIconSize, FSlateIcon& OutSlateIcon);

private:
	static const FName StyleSetName;

private:
	TSharedPtr<class FSlateStyleSet> StyleSet;

public:
	FSlateIcon WeaponIcon;

	TSharedPtr<FSlateImageBrush> DoActionDataBorder;
	TSharedPtr<FSlateImageBrush> DamagedDataBorder;
};
