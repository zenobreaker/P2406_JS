#pragma once

#include "CoreMinimal.h"


class SKILLEDITOR_API FSkillStyle
{
public:
	static void Regist();
	static TSharedPtr<FSkillStyle> Get();
	static void Unregist();

private:
	static TSharedPtr<FSkillStyle> Instance;

public:
	FSkillStyle();
	~FSkillStyle();

private:
	void RegisterIcon(const FString& InName, const FString& InPath, const FVector2D& InIconSize, FSlateIcon& OutSlateIcon);

private:
	static const FName StyleSetName;

private:
	TSharedPtr<class FSlateStyleSet> StyleSet;

public:
	FSlateIcon SkillIcon;

	TSharedPtr<FSlateImageBrush> DoActionDataBorder;
	TSharedPtr<FSlateImageBrush> DamagedDataBorder;
};
