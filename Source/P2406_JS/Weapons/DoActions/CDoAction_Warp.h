#pragma once

#include "CoreMinimal.h"
#include "Weapons/CDoAction.h"
#include "CDoAction_Warp.generated.h"


UCLASS()
class P2406_JS_API UCDoAction_Warp : public UCDoAction
{
	GENERATED_BODY()


public: 
	UCDoAction_Warp(); 

public:
	void BeginPlay
	(
		class ACharacter* InOwner,
		class ACAttachment* InAttachment,
		class UCEquipment* InEquipment,
		const TArray<FDoActionData>& InDoActionDatas,
		const TArray<FHitData>& InHitDatas
	) override;

	void Tick(float InDeltaTime) override; 

public:
	void DoAction() override;
	void Begin_DoAction() override;

private:
	bool GetCursorLocationAndRotation(FVector& OutLocation, FRotator& OutRotation);

private:
	class UDecalComponent* Decal;
	class APlayerController* PlayerController;

private:
	FVector MoveToLocation;

};
