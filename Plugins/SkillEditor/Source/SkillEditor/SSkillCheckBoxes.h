#pragma once

#include "CoreMinimal.h"


class SKILLEDITOR_API SSkillCheckBoxes
	: public TSharedFromThis<SSkillCheckBoxes>
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
	void CheckDefaultValue(int32 InIndex, int32 InDefaultValue);
	void CheckDefaultValue(int32 InIndex, bool InDefaultValue);
	void CheckDefaultValue(int32 InIndex, const FVector& InDefaultValue);
	void CheckDefaultValue(int32 InIndex, const FRotator& InDefaultValue);
	void CheckDefaultValue(int32 InIndex, const FString& InDefaultValue);

	void CheckDefaultDoActionData(int32 InIndex, const struct FDoActionData& InDefautVale);
	void CheckDefaultSkillEntityData(int32 InIndex, const struct FSkillEntityData& InDefaultValue);


private:
	void OnCheckStateChanged(ECheckBoxState InState, int32 InIndex);

public:
	void SetUtilities(TSharedPtr<class IPropertyUtilities> InUtilities);

private:
	TSharedPtr<class IPropertyUtilities> Utilities;


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
