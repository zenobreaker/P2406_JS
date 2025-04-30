#pragma once

#include "CoreMinimal.h"
#include "Skill/CSkillEntity.h"
#include "CSkillEntity_Decal.generated.h"

UCLASS()
class P2406_JS_API ACSkillEntity_Decal 
	: public ACSkillEntity
{
	GENERATED_BODY()
	
public:
	ACSkillEntity_Decal();

	void BeginPlay() override;

	void Tick(float DeltaSeconds) override;

	void SetVisibleDecal(bool InValue); 

	void GetCapturedLocationAndRotation(FVector& OutLocation, FRotator& OutRotation);

private:
	bool GetCursorLocationAndRotation(FVector& OutLocation, FRotator& OutRotation);

private:
	class UDecalComponent* Decal;
	class APlayerController* PlayerController;
};
