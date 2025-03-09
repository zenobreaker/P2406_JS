#pragma once

#include "CoreMinimal.h"
#include "Skill/CSkillEntity.h"
#include "CSkillEntity_Attachment.generated.h"


// Owner의 특정 소켓에 지정된 콜리전을 붙인다. 
UCLASS(BlueprintType)
class P2406_JS_API ACSkillEntity_Attachment : public ACSkillEntity
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = "Skill Entity")
	TArray<FName> SocketNames;

public: 
	FORCEINLINE void SetSocketNames(const TArray<FName>& InNames) { SocketNames = InNames; }
	FORCEINLINE TArray<FName> GetSocketNames() { return SocketNames; }

public:
	virtual void BeginPlay() override;;
};
