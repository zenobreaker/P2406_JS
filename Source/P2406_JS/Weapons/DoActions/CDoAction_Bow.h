#pragma once

#include "CoreMinimal.h"
#include "Weapons/CDoAction.h"
#include "CDoAction_Bow.generated.h"


UCLASS(Blueprintable)
class P2406_JS_API UCDoAction_Bow : public UCDoAction
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Arrow")
	TSubclassOf<class ACArrow> ArrowClass;

private:
	void CreateArrow();
	class ACArrow* GetAttachedArrow();
public:
	UCDoAction_Bow();

	virtual void BeginPlay
	(
		class ACharacter* InOwner,
		class ACAttachment* InAttachment,
		class UCEquipment* InEquipment,
		const TArray<FDoActionData>& InDoActionDatas,
		const TArray<FHitData>& InHitDatas
	) override;

	void Tick(float InDeltaTime) override; 

	void DoAction() override;
	void Begin_DoAction() override;
	void End_DoAction() override;

public:
	void OnBeginEquip() override;
	void OnUnequip() override;

public:
	void End_BowString();

private:
	UFUNCTION()
	void OnArrowHit(class AActor* InCauser, class ACharacter* InOtherCharacter);

	UFUNCTION()
	void OnArrowEndPlay(class ACArrow* InDestroyer);

private:
	class USkeletalMeshComponent* SkeletalMesh;
	class UPoseableMeshComponent* PoseableMesh;

private:
	TArray<class ACArrow*> Arrows;

private:
	FVector OriginLocation;

private:
	const bool* bEquipped;
	bool bAttachedString = true;

private:
	float* Bending;
};
