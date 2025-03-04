#include "Skill/ActiveSkills/Sword/CSwordSkill_DragonFall.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"
#include "Curves/CurveVector.h"

#include "Characters/CBaseCharacter.h"
#include "Components/CZoomComponent.h"

#define LOG_UCSwordSkill_DragonFall

UCSwordSkill_DragonFall::UCSwordSkill_DragonFall()
{
	FHelpers::GetAsset<UCurveVector>(&Curve,
		"/Script/Engine.CurveVector'/Game/Weapons/Sword/Sword_Soar_Curve.Sword_Soar_Curve'");
}

void UCSwordSkill_DragonFall::BeginPlay_ActiveSkill(ACharacter* InOwner, FSkillFlowData InFlowData)
{
	Super::BeginPlay_ActiveSkill(InOwner, InFlowData);

	CurrDFFlow = DrangonFallFlow::None;

	// 착지 이벤트 연결
	{
		Base = Cast<ACBaseCharacter>(InOwner);
	}

	// 입력 키 설정
	SetInputKey(EKeys::LeftMouseButton);

	// 컴포넌트
	{
		SpringArm = FHelpers::GetComponent<USpringArmComponent>(InOwner);
		Camera = FHelpers::GetComponent<UCameraComponent>(InOwner);
		Zoom = FHelpers::GetComponent<UCZoomComponent>(InOwner);

		SetOriginCameraData();
	}

	// 상승 구간 타임라인
	{
		FOnTimelineVector timeline;
		timeline.BindUFunction(this, "OnSoaring");
		SoarTimelineData.Timeline.AddInterpVector(Curve, timeline);

		FOnTimelineEvent finished;
		finished.BindUFunction(this, "OnTimelimeFinished");
		SoarTimelineData.Timeline.SetTimelineFinishedFunc(finished);
		SoarTimelineData.Timeline.SetPlayRate(SoarSpeed);
	}

	// 카메라용 타임라인
	{

		float min = 0.0f, max = 0.0f;
		float  min1 = 0.0f, max1 = 0.0f, min2 = 0.0f, max2 = 0.0f, min3 = 0.0f, max3 = 0.0f;
		Curve->FloatCurves[0].GetTimeRange(min1, max1);
		Curve->FloatCurves[1].GetTimeRange(min2, max2);
		Curve->FloatCurves[2].GetTimeRange(min3, max3);
		max = FMath::Max(max1, FMath::Max(max2, max3));

		// 카메라 타임라인이 끝나면 다음 분기로 보낼거임 
		FOnTimelineVector timeline;
		timeline.BindUFunction(this, "OnCameraZoomInOut");

		CameraTimelineData.MaxValue = max;
		CameraTimelineData.Timeline.AddInterpVector(Curve, timeline);
		CameraTimelineData.Timeline.SetPlayRate(PlayRate);
		CameraTimelineData.bTimelineFinished = false;
	}
}


void UCSwordSkill_DragonFall::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);

	// 입력페이즈면 입력확인
	if (CurrentPhase == ESkillPhase::WaitingForInput && bCanInput == false)
	{
		if (CheckPressedSetKey() == true)
		{
			bCanInput = true;
			OnChangeNextSkillPhase();
		}
	}

	SoarTimelineData.Timeline.TickTimeline(InDeltaTime);
	
	if (FMath::IsNearlyEqual(SoarTimelineData.Timeline.GetPlaybackPosition(),
		SoarTimelineData.Timeline.GetScaledTimelineLength(),
		KINDA_SMALL_NUMBER)
		&& SoarTimelineData.bTimelineFinished == false)
	{
		SoarTimelineData.bTimelineFinished = true;
		//GLog->Log("SoarTimelineData Timeline Finish");
	}


	CameraTimelineData.Timeline.TickTimeline(InDeltaTime);
	float v = CameraTimelineData.Timeline.GetPlaybackPosition();
	if (v >= CameraTimelineData.MaxValue
		&& CameraTimelineData.bTimelineFinished == false)
	{
		CameraTimelineData.bTimelineFinished = true;
		//GLog->Log("CameraTimelineData Timeline Finish");

		CameraFinished();
	}

}

//-----------------------------------------------------------------------------

ESkillPhase UCSwordSkill_DragonFall::GetNextFlowPhase(ESkillPhase InPhase)
{
	switch (InPhase)
	{
		case ESkillPhase::Start:
			return  ESkillPhase::Begin_Casting;
		case ESkillPhase::Begin_Casting:
			return  ESkillPhase::End_Casting;
		case ESkillPhase::End_Casting:
			return  ESkillPhase::WaitingForInput;
			// 입력 페이즈 추가 
		case ESkillPhase::WaitingForInput:
			return ESkillPhase::Begin_Skill;

		case ESkillPhase::Begin_Skill:
			return ESkillPhase::End_Skill;
		case ESkillPhase::End_Skill:
			return ESkillPhase::Finished;
	}

	return ESkillPhase::Max;
}

void UCSwordSkill_DragonFall::DefineSkillPhases()
{
	Super::DefineSkillPhases();

	// 기존 스킬 플로우에서 추가만함 
	AssignSkillPhase(ESkillPhase::Start,
		[this]() { Start_Skill(); });

	// 입력 페이즈 추가 
	AssignSkillPhase(ESkillPhase::WaitingForInput,
		[this]() { Input_AnyKey(); });
}

void UCSwordSkill_DragonFall::OnPressedKey()
{
	//  지정한 키가 입력되었으므로 다음 페이즈 
	OnChangeNextSkillPhase();
}

void UCSwordSkill_DragonFall::OnReleasedKey()
{

}

void UCSwordSkill_DragonFall::Create_Collision()
{
	RunSkillPhaseData(ESkillPhase::End_Skill);
}


void UCSwordSkill_DragonFall::Start_Skill()
{
	// 타이머 핸들 초기화 
	{
		// 해당 타이머 핸들 기능 제거 
		OwnerCharacter->GetWorld()->GetTimerManager().ClearTimer(SoarTimelineData.Handle);
		OwnerCharacter->GetWorld()->GetTimerManager().ClearTimer(CameraTimelineData.Handle);

		SoarTimelineData.Handle.Invalidate();
		CameraTimelineData.Handle.Invalidate();

		SoarTimelineData.bTimelineFinished = false; 
		CameraTimelineData.bTimelineFinished = false; 
	}

	// 자주 쓰이는 것이 아니니까 이벤트를 다시 생성 
	REGISTER_EVENT_WITH_REPLACE(Base, OnCharacterLanded, this, UCSwordSkill_DragonFall::OnLanded);


	bCanInput = false;
	bEndSkillCall = false;

	CameraLocation = FVector::ZeroVector;
	OriginVector = OwnerCharacter->GetActorLocation();

	ExecutePhase(ESkillPhase::Begin_Casting);
}

void UCSwordSkill_DragonFall::Begin_Casting()
{
	Super::Begin_Casting();

	//SkillPhaseTable[ESkillPhase::Begin_Casting].PhaseDatas[0].ExecutePhase(OwnerCharacter);

	// 카메라 일정거리로 앞당기기 
	// 카메라 타임라인이 수행되고 나면 다음 분기로 이동시키는 함수가 호출된다.
	SetCameraData();
}


// 캐릭터가 상승하고 난 후 
void UCSwordSkill_DragonFall::End_Casting()
{
	Super::End_Casting();

	// 캐릭터를 상승시킨다. 
	RunSkillPhaseData(ESkillPhase::End_Casting);
	// 타임라인 수행 
	if (SoarTimelineData.Timeline.IsPlaying() == false)
	{
		SoarTimelineData.Timeline.PlayFromStart();
	}
}


void UCSwordSkill_DragonFall::Input_AnyKey()
{
#ifdef  LOG_UCSwordSkill_DragonFall
	FLog::Log("DF - Input AnyKey Phase");
#endif
	// 부모 내용 호출 
	Super::Input_AnyKey();
}

void UCSwordSkill_DragonFall::Begin_Skill()
{
	Super::Begin_Skill();

#ifdef  LOG_UCSwordSkill_DragonFall
	FLog::Log("DF - Begin Skill Phase");
#endif

	//OnSkillDoAction 

	RunSkillPhaseData(ESkillPhase::Begin_Skill);
}

void UCSwordSkill_DragonFall::End_Skill()
{
	Super::End_Skill();

#ifdef  LOG_UCSwordSkill_DragonFall
	FLog::Log("DF - End Skill Phase");
#endif

	if (bEndSkillCall == true)
	{
		CurrentPhase = ESkillPhase::Max;
		return;
	}

	// 카메라 리셋 
	ResetCameraData();

	bEndSkillCall = true; 
	
	// 그냥 착지..
	// 이펙트랑 사운드 빼고 재생 
	//RunSkillPhaseData(ESkillPhase::End_Skill);
	//SkillPhaseTable[ESkillPhase::End_Skill].PhaseDatas[0].Phase_DoAction(OwnerCharacter);

	OnChangeNextSkillPhase();
}

void UCSwordSkill_DragonFall::Finish_Skill()
{
	UNREGISTER_EVENT(Base, OnCharacterLanded, this, UCSwordSkill_DragonFall::OnLanded);
	
	OwnerCharacter->GetWorld()->GetTimerManager().ClearTimer(SoarTimelineData.Handle);
	OwnerCharacter->GetWorld()->GetTimerManager().ClearTimer(CameraTimelineData.Handle);

	RestoreGravity();
	ReleaseKey();

	Super::Finish_Skill();
}


void UCSwordSkill_DragonFall::OnSkillDoAction()
{
	// 입력되어 진 카메라 방향으로 캐릭터를 사출한다.
	OnDescent();
}


void UCSwordSkill_DragonFall::RestoreGravity()
{
	if (OwnerCharacter)
	{
		OwnerCharacter->GetCharacterMovement()->GravityScale = 1.0f; // 중력 복구
	}
}

void UCSwordSkill_DragonFall::OnDescent()
{
	CheckNull(OwnerCharacter);

	// 저장해놓은 카메라의 전방벡터로 날아간다. 
	FVector forward = Camera->GetForwardVector(); // 또는 CameraRotator.GetForwardVector();
	forward.Normalize();
	
	// 카메라 리셋 
	ResetCameraData();

	// 캐릭터 발사 
	OwnerCharacter->LaunchCharacter(forward * DescentSpeed, true, true);
}


void UCSwordSkill_DragonFall::OnSoaring(FVector Output)
{
	CheckNull(OwnerCharacter);

	FVector location = OriginVector;
	location.Z += Output.Z;
	OwnerCharacter->SetActorLocation(location);

	if (CurrentPhase == ESkillPhase::End_Casting)
	{
		float playRate = SoarTimelineData.Timeline.GetPlayRate();
		// 타임라인 수행하면서 지정한 애니메이션 동작 
		SkillPhaseTable[ESkillPhase::End_Casting]
			.PhaseDatas[0].Phase_AnimationPlayback(OwnerCharacter, playRate);
	}
}

void UCSwordSkill_DragonFall::OnTimelimeFinished()
{
#ifdef LOG_UCSwordSkill_DragonFall
	FLog::Log("DF - TimelineFinish");
#endif

	// 입력 페이즈로 다 올랐으면  -> 입력페이즈로 갈거임 
	OnChangeNextSkillPhase();

	// 중력 없애기
	OwnerCharacter->GetCharacterMovement()->GravityScale = 0.0f;
	if (OwnerCharacter->GetWorld()->GetTimerManager().IsTimerActive(SoarTimelineData.Handle) == false)
	{
		OwnerCharacter->GetWorld()->GetTimerManager().SetTimer
		(
			SoarTimelineData.Handle,
			this,
			&UCSwordSkill_DragonFall::RestoreGravity,
			HoldDuration,
			false
		);
	}
}

void UCSwordSkill_DragonFall::OnLanded()
{
	CheckNull(OwnerCharacter);
#ifdef LOG_UCSwordSkill_DragonFall
	FLog::Log("DF - OnLanded");
#endif
	if (bIsAction == false)
		return; 


	// 충격파 생성 - 입력했을 때만 (정확히는 모션이 수행되고 나서지만..)
	if (bCanInput == true)
	{
		RunSkillPhaseData(ESkillPhase::End_Skill);
		OnActivated_Collision();
	}
	
	// 착지되면 강제로 스킬 종료 
	// Notify 에게 맡긴다... 
	ExecutePhase(ESkillPhase::End_Skill);
}

void UCSwordSkill_DragonFall::CameraFinished()
{
#ifdef  LOG_UCSwordSkill_DragonFall
	GLog->Log("CameraTimelineData Timeline Finish to NextPhase ");
#endif

	CameraTimelineData.Timeline.Stop();

	OnChangeNextSkillPhase(CurrentPhase);
}

void UCSwordSkill_DragonFall::OnCameraZoomInOut(FVector Output)
{
	CheckNull(Camera);

	FLog::Print(FString::SanitizeFloat(Output.Y), 25270, 5.0f, FColor::Orange);
	Camera->FieldOfView = Output.Y;
}

void UCSwordSkill_DragonFall::SetOriginCameraData()
{
	CheckNull(SpringArm);
	CheckNull(Camera);

	// 현재 값 저장 
	OriginData.TargetArmLength = SpringArm->TargetArmLength;
	OriginData.SocketOffset = SpringArm->SocketOffset;
	OriginData.bEnableCameraLag = SpringArm->bEnableCameraLag;
	OriginData.CameraLocation = Camera->GetRelativeLocation();
}

void UCSwordSkill_DragonFall::SetCameraData()
{
	CheckNull(SpringArm);
	CheckNull(Camera);

	// 줌 끄기 
	if (Zoom != nullptr)
	{
		Zoom->SetZoomOff();
	}

	// 현재 값 저장 
	OriginData.TargetArmLength = SpringArm->TargetArmLength;
	OriginData.SocketOffset = SpringArm->SocketOffset;
	OriginData.bEnableCameraLag = SpringArm->bEnableCameraLag;
	OriginData.CameraLocation = Camera->GetRelativeLocation();
	
	SpringArm->TargetArmLength = AimData.TargetArmLength;
	SpringArm->SocketOffset = AimData.SocketOffset;
	SpringArm->bEnableCameraLag = AimData.bEnableCameraLag;
	Camera->SetRelativeLocation(AimData.CameraLocation);

	CameraTimelineData.Timeline.PlayFromStart();

}

void UCSwordSkill_DragonFall::ResetCameraData()
{
	CheckNull(SpringArm);
	CheckNull(Camera);

	// 줌 켜기 
	if (Zoom != nullptr)
	{
		Zoom->SetZoomOn();
	}

	SpringArm->TargetArmLength = OriginData.TargetArmLength;
	SpringArm->SocketOffset = OriginData.SocketOffset;
	SpringArm->bEnableCameraLag = OriginData.bEnableCameraLag;
	Camera->SetRelativeLocation(OriginData.CameraLocation);

	//Camera->FieldOfView = 90;
	//CameraTimeline.ReverseFromEnd();
}
