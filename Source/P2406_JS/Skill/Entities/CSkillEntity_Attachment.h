#pragma once

#include "CoreMinimal.h"
#include "Skill/CSkillEntity.h"
#include "CSkillEntity_Attachment.generated.h"


// Owner�� Ư�� ���Ͽ� ������ �ݸ����� ���δ�. 
UCLASS(BlueprintType)
class P2406_JS_API ACSkillEntity_Attachment : public ACSkillEntity
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = "Skill Entity")
	TArray<FName> SocketNames;

public:
	virtual void BeginPlay() override;

public:
	void ActivateCollision(FName InName) override;
	void DeactivateCollision(FName InName) override;

private:
	TArray<class UCSkillCollisionComponent*> SkillCollisions;
	
};
