#pragma once

#include "CoreMinimal.h"
#include "InputIconData.generated.h"


USTRUCT(BlueprintType)
struct FInputIconData : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FKey InputKey;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* IconImage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Description;
};


class P2406_JS_API InputIconData
{
public:
};
