#include "Skill/ActiveSkills/Sword/CSwordSkill_DragonFall.h"
#include "Global.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"

UCSwordSkill_DragonFall::UCSwordSkill_DragonFall()
{
	CHelpers::GetAsset<UCurveVector>(&Curve,
		"/Script/Engine.CurveVector'/Game/Weapons/Sword/Sword_Soar_Curve.Sword_Soar_Curve'");
}

void UCSwordSkill_DragonFall::BeginPlay(ACharacter* InOwner, const TArray<FSkillActionData>& InDoActionDatas, const TArray<FSkillHitData>& InHitDatas)
{
	Super::BeginPlay(InOwner, InDoActionDatas, InHitDatas);

	CLog::Print("Dragon Fall Begin Play!!!!!" + OwnerCharacter->GetName()); 


	// 컴포넌트
	{
		SpringArm = CHelpers::GetComponent<USpringArmComponent>(InOwner);
		Camera = CHelpers::GetComponent<UCameraComponent>(InOwner);
	}

	// 상승 구간 타임라인
	{
		FOnTimelineVector timeline;
		timeline.BindUFunction(this, "OnSoaring");
		Timeline.AddInterpVector(Curve, timeline);

		FOnTimelineEvent finished;
		finished.BindUFunction(this, "OnTimelimeFinished");
		Timeline.SetTimelineFinishedFunc(finished);
		Timeline.SetPlayRate(SoarSpeed);
	}

	// 카메라용 타임라인
	{
		FOnTimelineVector timeline;
		timeline.BindUFunction(this, "OnCameraZoomInOut");
		CameraTimeline.AddInterpVector(Curve, timeline);
		CameraTimeline.SetPlayRate(200.0f);
	}
}

void UCSwordSkill_DragonFall::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);
	CLog::Print("Skill Tick Call : " + OwnerCharacter->GetName(), 2, 10.0f, FColor::Green);
	
	Timeline.TickTimeline(InDeltaTime);
	CameraTimeline.TickTimeline(InDeltaTime);
}

void UCSwordSkill_DragonFall::OnSoarCharacter()
{
	CheckNull(OwnerCharacter); 

	if (Timeline.IsPlaying() == false)
	{
		Timeline.PlayFromStart();
	}


	if (OnSoaringBegin.IsBound())
		OnSoaringBegin.Broadcast(); 

}

void UCSwordSkill_DragonFall::OnDescent()
{
	CheckNull(OwnerCharacter); 

	// 저장해놓은 카메라의 전방벡터로 날아간다. 
	FVector direction = CameraRotator.Vector(); // 또는 CameraRotator.GetForwardVector();
	direction.Normalize();
	ForwardVector.Normalize();

	FVector start = OwnerCharacter->GetActorLocation();
	FVector end = start + direction * 500.0f;

	DrawDebugLine(OwnerCharacter->GetWorld(), start, end, FColor::Green, false, 5.0f);


	OwnerCharacter->LaunchCharacter(ForwardVector * DescentSpeed, true, true);
	 

}



void UCSwordSkill_DragonFall::Begin_Casting()
{
	bClickedKey = false;

	CameraLocation = FVector::ZeroVector;
	CameraRotator = FRotator::ZeroRotator;

	OriginVector = OwnerCharacter->GetActorLocation();

	// 타이머 핸들 초기화 
	{
		// 해당 타이머 핸들 기능 제거 
		HoldTimerHandle.Invalidate();

		LandCheckTimerHandle.Invalidate();
	}

	// 카메라 일정거리로 앞당기기 
	SetCameraData();

	DoActionDatas[Index].Begin_Casting(OwnerCharacter);
}



void UCSwordSkill_DragonFall::End_Casting()
{
	DoActionDatas[Index].End_Casting(OwnerCharacter, true);

	// 착지했는지 검사 
	OwnerCharacter->GetWorld()->GetTimerManager().SetTimer(LandCheckTimerHandle,
		this, &UCSwordSkill_DragonFall::OnCheckIfLand, 0.1f, false);
	if (OnSoaringEnd.IsBound())
		OnSoaringEnd.Broadcast();

	StartNextPhase();
}



void UCSwordSkill_DragonFall::Input_AnyKey()
{
	// 카메라 방향을 저장한다. 
	CheckNull(OwnerCharacter); 
	APlayerController* controller = OwnerCharacter->GetController<APlayerController>();
	CheckNull(controller); 
	
	controller->GetPlayerViewPoint(CameraLocation, CameraRotator);

	ForwardVector = CameraRotator.Vector(); 
	bClickedKey = true; 
	
	// 부모 내용 호출 
	Super::Input_AnyKey(); 
}


void UCSwordSkill_DragonFall::OffSkillCasting()
{
	CLog::Print("Off Skill Casting Call Complete");

	// 여기서 다음 페이즈로	
	OnSoarCharacter();
}

void UCSwordSkill_DragonFall::OnSkillDoAction()
{
	// 입력되어 진 카메라 방향으로 캐릭터를 사출한다.
	OnDescent();
}

void UCSwordSkill_DragonFall::OffSkillDoAction()
{

}

void UCSwordSkill_DragonFall::RestoreGravity()
{
	if (OwnerCharacter)
	{
		OwnerCharacter->GetCharacterMovement()->GravityScale = 1.0f; // 중력 복구
	}


}

void UCSwordSkill_DragonFall::OnSoaring(FVector Output)
{
	CheckNull(OwnerCharacter); 
	
	//CLog::Log("OutPutZ : " + OwnerCharacter->GetName());
	OriginVector.Z = Output.Z;
	OwnerCharacter->SetActorLocation(OriginVector);
	
	// 중력 없애기
	OwnerCharacter->GetCharacterMovement()->GravityScale = 0.0f;

	if (!OwnerCharacter->GetWorld()->GetTimerManager().IsTimerActive(HoldTimerHandle))
	{
		OwnerCharacter->GetWorld()->GetTimerManager().SetTimer(
			HoldTimerHandle, this, &UCSwordSkill_DragonFall::RestoreGravity, HoldDuration, false);
	}
}

void UCSwordSkill_DragonFall::OnTimelimeFinished()
{
	StartNextPhase();
}

void UCSwordSkill_DragonFall::OnCheckIfLand()
{
	CheckNull(OwnerCharacter); 

	bool bCheck = OwnerCharacter->GetCharacterMovement()->IsFalling() == false;
	if (bCheck)
	{
		// 여기서도 중력 복구 시켜봄
		RestoreGravity();
		// 착지하면 다음 페이즈 
		CLog::Print("Skill - Landed Complete");

		// 착지하면 키를 눌렀는지에 따라 애니메이션 동작 다르게
		// 그냥 누르지 않고 떨어졌다면 랜딩 몽타주 섹션 스타트 
		if (bClickedKey == false)
		{
			//TODO: 섹션 네임을 데이터에 저장하고 그걸 불러오자 
			DoActionDatas[Index].PlaySection_SkillCastingMontage(OwnerCharacter, 1.0f, "EndCasting");

			// 그냥 착지했으면 스킬  끝내는 페이즈로 보낸다. 
			StartNextPhase(); 
		}
		// 공격키가 눌러진 상태로 땅에 착지하면 그 애니메이션
		else
		{
			//TODO: 스킬 기능 정리가 끝나면 이 내용도 별도의 함수나 그런걸로..?
			// 소리 출력 
			DoActionDatas[Index].PlaySoundWave(OwnerCharacter);
			
			// 충격파 생성
			Create_Collision(); 

			// 여기선 착지 모션이 나오고 그 모션이 끝나야 스킬 엔드처리해야한다. 
			DoActionDatas[Index].PlaySecion_SkillActionMontage(OwnerCharacter, 1.0f, "EndSkill");
		}

		// 카메라 리셋 
		ResetCameraData(); 

		LandCheckTimerHandle.Invalidate(); 
	}
	else
	{
		// 착지하지 않으면 계속 체크 
		OwnerCharacter->GetWorld()->GetTimerManager().SetTimer(LandCheckTimerHandle,
			this, &UCSwordSkill_DragonFall::OnCheckIfLand, 0.1f, false);
	}
}

void UCSwordSkill_DragonFall::OnCameraZoomInOut(FVector Output)
{
	CheckNull(Camera);

	Camera->FieldOfView = Output.Y;
}

void UCSwordSkill_DragonFall::SetCameraData()
{
	CheckNull(SpringArm);
	CheckNull(Camera);

	// 현재 값 저장 
	OriginData.TargetArmLength = SpringArm->TargetArmLength;
	OriginData.SocketOffset = SpringArm->SocketOffset;
	OriginData.bEnableCameraLag = SpringArm->bEnableCameraLag;
	OriginData.CameraLocation = Camera->GetRelativeLocation();

	SpringArm->TargetArmLength = AimData.TargetArmLength;
	SpringArm->SocketOffset = AimData.SocketOffset;
	SpringArm->bEnableCameraLag = AimData.bEnableCameraLag;
	Camera->SetRelativeLocation(AimData.CameraLocation);

	CameraTimeline.PlayFromStart();
}

void UCSwordSkill_DragonFall::ResetCameraData()
{
	CheckNull(SpringArm);
	CheckNull(Camera);

	SpringArm->TargetArmLength = OriginData.TargetArmLength;
	SpringArm->SocketOffset = OriginData.SocketOffset;
	SpringArm->bEnableCameraLag = OriginData.bEnableCameraLag;
	Camera->SetRelativeLocation(OriginData.CameraLocation);

	Camera->FieldOfView = 90;
	//CameraTimeline.ReverseFromEnd();
}
