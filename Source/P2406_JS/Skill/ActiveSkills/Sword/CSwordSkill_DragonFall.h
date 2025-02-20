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
	FVector SocketOffset = FVector(30, 30, 10);

	UPROPERTY(EditAnywhere)
	bool bEnableCameraLag = false;

	UPROPERTY(EditAnywhere)
	FVector CameraLocation = FVector::OneVector;
};

UENUM()
enum class DrangonFallFlow
{
	None = -1,
	BC_CamaraZoomIn = 0,//Begin_Casting
	BC_Rising,	// Begin_Casting
	IW_Hover,  // InputForWait 
	Landing,	// End_Skill
	End,		// Finish
};


UCLASS()
class P2406_JS_API UCSwordSkill_DragonFall
	: public UCActiveSkill_Stance
{
	GENERATED_BODY()

public:
	UCSwordSkill_DragonFall();

public:
	UPROPERTY(EditAnywhere)
	class UCurveVector* Curve;

	UPROPERTY(EditAnywhere)
	float PlayRate = 1.0f;

	UPROPERTY(EditAnywhere)
	float HoldDuration = 5.0f;

	UPROPERTY(EditAnywhere)
	FDFAimData AimData;

public:
	void BeginPlay_ActiveSkill(ACharacter* InOwner, FSkillFlowData InFlowData) override;

	void Tick(float InDeltaTime) override;


protected:
	ESkillPhase GetNextFlowPhase(ESkillPhase InPhase) override;
	void DefineSkillPhases() override;

	void OnPressedKey() override;
	void OnReleasedKey() override;


public:
	void Start_Skill() override;
	void Begin_Casting() override;
	void End_Casting() override;
	// 특정 키 입력 
	void Input_AnyKey() override;
	void Begin_Skill() override; 
	void End_Skill() override; 
	void Finish_Skill() override;

public:
	void OnSkillDoAction() override;

	void RestoreGravity();
	
	void OnDescent();

private:
	UFUNCTION()
	void OnSoaring(FVector Output);
	UFUNCTION()
	void OnTimelimeFinished();
	UFUNCTION()
	void OnLanded();

	void CameraFinished();
private:
	void SetOriginCameraData();
	void SetCameraData();
	void ResetCameraData();

private:
	UFUNCTION()
	void OnCameraZoomInOut(FVector Output);


private:
	float SoarSpeed = 1000.0f;
	float DescentSpeed = 10000.0f;
	float lmitCameraPitch = 80.0f;
	float originCameraPtich = 0.0f;

private:
	float CameraSpeed = 1250.0f;


private:
	struct InteralTimelineData
	{
		FTimeline Timeline;
		FTimerHandle  Handle;
		bool bTimelineFinished;

		float MaxValue = 0.0f;
	};
	InteralTimelineData SoarTimelineData;
	InteralTimelineData CameraTimelineData;

private:
	class USpringArmComponent* SpringArm;
	class UCameraComponent* Camera;
	class UCZoomComponent* Zoom;

private:
	bool bClickedKey = false;
	bool bEndSkillCall = false; //TODO : 하드 코딩의 표본 임시로 막는 거니 방법을 생각 해야한다.

private:
	DrangonFallFlow CurrDFFlow;
	FDFAimData OriginData;

	FVector ForwardVector;
	FVector OriginVector;
	FVector CameraLocation;// 입력되었을 때 방향 
	FRotator CameraRotator;

};
