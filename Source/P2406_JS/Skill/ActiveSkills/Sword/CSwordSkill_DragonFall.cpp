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

	// ���� �̺�Ʈ ����
	{
		Base = Cast<ACBaseCharacter>(InOwner);
	}

	// �Է� Ű ����
	SetInputKey(EKeys::LeftMouseButton);

	// ������Ʈ
	{
		SpringArm = FHelpers::GetComponent<USpringArmComponent>(InOwner);
		Camera = FHelpers::GetComponent<UCameraComponent>(InOwner);
		Zoom = FHelpers::GetComponent<UCZoomComponent>(InOwner);

		SetOriginCameraData();
	}

	// ��� ���� Ÿ�Ӷ���
	{
		FOnTimelineVector timeline;
		timeline.BindUFunction(this, "OnSoaring");
		SoarTimelineData.Timeline.AddInterpVector(Curve, timeline);

		FOnTimelineEvent finished;
		finished.BindUFunction(this, "OnTimelimeFinished");
		SoarTimelineData.Timeline.SetTimelineFinishedFunc(finished);
		SoarTimelineData.Timeline.SetPlayRate(SoarSpeed);
	}

	// ī�޶�� Ÿ�Ӷ���
	{

		float min = 0.0f, max = 0.0f;
		float  min1 = 0.0f, max1 = 0.0f, min2 = 0.0f, max2 = 0.0f, min3 = 0.0f, max3 = 0.0f;
		Curve->FloatCurves[0].GetTimeRange(min1, max1);
		Curve->FloatCurves[1].GetTimeRange(min2, max2);
		Curve->FloatCurves[2].GetTimeRange(min3, max3);
		max = FMath::Max(max1, FMath::Max(max2, max3));

		// ī�޶� Ÿ�Ӷ����� ������ ���� �б�� �������� 
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

	// �Է�������� �Է�Ȯ��
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
			// �Է� ������ �߰� 
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

	// ���� ��ų �÷ο쿡�� �߰����� 
	AssignSkillPhase(ESkillPhase::Start,
		[this]() { Start_Skill(); });

	// �Է� ������ �߰� 
	AssignSkillPhase(ESkillPhase::WaitingForInput,
		[this]() { Input_AnyKey(); });
}

void UCSwordSkill_DragonFall::OnPressedKey()
{
	//  ������ Ű�� �ԷµǾ����Ƿ� ���� ������ 
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
	// Ÿ�̸� �ڵ� �ʱ�ȭ 
	{
		// �ش� Ÿ�̸� �ڵ� ��� ���� 
		OwnerCharacter->GetWorld()->GetTimerManager().ClearTimer(SoarTimelineData.Handle);
		OwnerCharacter->GetWorld()->GetTimerManager().ClearTimer(CameraTimelineData.Handle);

		SoarTimelineData.Handle.Invalidate();
		CameraTimelineData.Handle.Invalidate();

		SoarTimelineData.bTimelineFinished = false; 
		CameraTimelineData.bTimelineFinished = false; 
	}

	// ���� ���̴� ���� �ƴϴϱ� �̺�Ʈ�� �ٽ� ���� 
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

	// ī�޶� �����Ÿ��� �մ��� 
	// ī�޶� Ÿ�Ӷ����� ����ǰ� ���� ���� �б�� �̵���Ű�� �Լ��� ȣ��ȴ�.
	SetCameraData();
}


// ĳ���Ͱ� ����ϰ� �� �� 
void UCSwordSkill_DragonFall::End_Casting()
{
	Super::End_Casting();

	// ĳ���͸� ��½�Ų��. 
	RunSkillPhaseData(ESkillPhase::End_Casting);
	// Ÿ�Ӷ��� ���� 
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
	// �θ� ���� ȣ�� 
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

	// ī�޶� ���� 
	ResetCameraData();

	bEndSkillCall = true; 
	
	// �׳� ����..
	// ����Ʈ�� ���� ���� ��� 
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
	// �ԷµǾ� �� ī�޶� �������� ĳ���͸� �����Ѵ�.
	OnDescent();
}


void UCSwordSkill_DragonFall::RestoreGravity()
{
	if (OwnerCharacter)
	{
		OwnerCharacter->GetCharacterMovement()->GravityScale = 1.0f; // �߷� ����
	}
}

void UCSwordSkill_DragonFall::OnDescent()
{
	CheckNull(OwnerCharacter);

	// �����س��� ī�޶��� ���溤�ͷ� ���ư���. 
	FVector forward = Camera->GetForwardVector(); // �Ǵ� CameraRotator.GetForwardVector();
	forward.Normalize();
	
	// ī�޶� ���� 
	ResetCameraData();

	// ĳ���� �߻� 
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
		// Ÿ�Ӷ��� �����ϸ鼭 ������ �ִϸ��̼� ���� 
		SkillPhaseTable[ESkillPhase::End_Casting]
			.PhaseDatas[0].Phase_AnimationPlayback(OwnerCharacter, playRate);
	}
}

void UCSwordSkill_DragonFall::OnTimelimeFinished()
{
#ifdef LOG_UCSwordSkill_DragonFall
	FLog::Log("DF - TimelineFinish");
#endif

	// �Է� ������� �� �ö�����  -> �Է�������� ������ 
	OnChangeNextSkillPhase();

	// �߷� ���ֱ�
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


	// ����� ���� - �Է����� ���� (��Ȯ���� ����� ����ǰ� ��������..)
	if (bCanInput == true)
	{
		RunSkillPhaseData(ESkillPhase::End_Skill);
		OnActivated_Collision();
	}
	
	// �����Ǹ� ������ ��ų ���� 
	// Notify ���� �ñ��... 
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

	// ���� �� ���� 
	OriginData.TargetArmLength = SpringArm->TargetArmLength;
	OriginData.SocketOffset = SpringArm->SocketOffset;
	OriginData.bEnableCameraLag = SpringArm->bEnableCameraLag;
	OriginData.CameraLocation = Camera->GetRelativeLocation();
}

void UCSwordSkill_DragonFall::SetCameraData()
{
	CheckNull(SpringArm);
	CheckNull(Camera);

	// �� ���� 
	if (Zoom != nullptr)
	{
		Zoom->SetZoomOff();
	}

	// ���� �� ���� 
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

	// �� �ѱ� 
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
