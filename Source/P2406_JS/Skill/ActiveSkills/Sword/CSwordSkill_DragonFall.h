#pragma once

#include "CoreMinimal.h"
#include "Skill/ActiveSkills/CActiveSkill_Stance.h"
#include "Components/TimelineComponent.h"
#include "CSwordSkill_DragonFall.generated.h"

USTRUCT()
struct FDFAimData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float TargetArmLength = 100;

	UPROPERTY(EditAnywhere)
	FVector SocketOffset = FVector(0, 30, 10);

	UPROPERTY(EditAnywhere)
	bool bEnableCameraLag = false;

	UPROPERTY(EditAnywhere)
	FVector CameraLocation = FVector::OneVector;
};



UCLASS()
class P2406_JS_API UCSwordSkill_DragonFall : public UCActiveSkill_Stance
{
	GENERATED_BODY()

public:
	UCSwordSkill_DragonFall();
	 


public:
	UPROPERTY(EditAnywhere)
	class UCurveVector* Curve;

	UPROPERTY(EditAnywhere)
	float HoldDuration = 5.0f; 

	UPROPERTY(EditAnywhere)
	FDFAimData AimData;

public:
	virtual void BeginPlay(
		ACharacter* InOwner,
		const TArray<FSkillActionData>& InDoActionDatas,
		const TArray<FSkillHitData>& InHitDatas) override;

	virtual void Tick(float InDeltaTime) override;


public:
	void OnSoarCharacter();
	void OnDescent();

public:
	void Begin_Casting() override; 
	void End_Casting() override; 

	//void Create_Collision() override; 
public:
	// 특정 키 입력 
	virtual void Input_AnyKey() override;


public:
	virtual void OffSkillCasting() override;

	virtual void OnSkillDoAction() override; 
	virtual void OffSkillDoAction() override; 

	void RestoreGravity();

private:
	float SoarSpeed = 1000.0f; 
	
	float DescentSpeed = 10000.0f; 

	float lmitCameraPitch = 80.0f; 

	float originCameraPtich = 0.0f; 

private:
	float CameraSpeed = 1250.0f; 

private: 
	UFUNCTION()
	void OnSoaring(FVector Output);
	UFUNCTION()
	void OnTimelimeFinished();
	UFUNCTION()
	void OnCheckIfLand(); 


private:
	void SetCameraData();
	void ResetCameraData(); 

private:
	UFUNCTION()
	void OnCameraZoomInOut(FVector Output);

private:
	FTimeline Timeline;
	FTimeline CameraTimeline;

	FTimerHandle HoldTimerHandle; 
	FTimerHandle LandCheckTimerHandle; 

private: 
	class USpringArmComponent* SpringArm;
	class UCameraComponent* Camera;

private: 
	bool bClickedKey = false; 

private:
	FDFAimData OriginData;

	FVector ForwardVector;
	FVector OriginVector;
	FVector CameraLocation;// 입력되었을 때 방향 
	FRotator CameraRotator;
 
};
