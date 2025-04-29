#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CStatComponent.h"
#include "CUserWidget_CharacterInfo.generated.h"

UCLASS()
class P2406_JS_API UCUserWidget_CharacterInfo : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void DrawInfo(); 

	UFUNCTION(BlueprintCallable)
	void SetStat(class ACharacter* InOwer); 


public:
	UPROPERTY(BlueprintReadOnly)
	class UCStatComponent* Stat; 

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	struct FStatusData StatusInfo; 
};
