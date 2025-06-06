#pragma once

#include "CoreMinimal.h"

class WEAPONEDITOR_API SWeaponCheckBoxes
	: public TSharedFromThis<SWeaponCheckBoxes>
{
public:
	void AddProperties(TSharedPtr<IPropertyHandle> InHandle);

public:
	static bool CanDraw(TSharedPtr<class IPropertyHandle> InHandle, int32 InCount);

public:
	TSharedRef<SWidget> DrawHeader();
	void DrawProperties(TSharedRef<IPropertyHandle> InPropertyHandle, IDetailChildrenBuilder* InChildBuilder);

public:
	void CheckDefaultObject(int32 InIndex, UObject* InDefaultValue);
	void CheckDefaultValue(int32 InIndex, float InDefaultValue);
	void CheckDefaultValue(int32 InIndex, bool InDefaultValue);
	void CheckDefaultValue(int32 InIndex, const FVector& InDefaultValue);

private:
	void OnCheckStateChanged(ECheckBoxState InState, int32 InIndex);

public:
	void SetUtilities(TSharedPtr<class IPropertyUtilities> InUtilities);

private:
	TSharedPtr<class IPropertyUtilities> Utilities;
	bool bShouldShowEffectLocationScale;
	bool bDrawComplete = false;

private:
	struct FInternalData
	{
		bool bChecked;
		FString Name;
		TSharedPtr<IPropertyHandle> Handle;

		FInternalData(TSharedPtr<IPropertyHandle> InHandle)
		{
			bChecked = false;
			Handle = InHandle;
			Name = Handle->GetPropertyDisplayName().ToString();
		}
	};
	TArray<FInternalData> InternalDatas;
};
