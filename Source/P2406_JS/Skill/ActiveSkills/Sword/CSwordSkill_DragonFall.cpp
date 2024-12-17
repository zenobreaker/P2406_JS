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


	// ������Ʈ
	{
		SpringArm = CHelpers::GetComponent<USpringArmComponent>(InOwner);
		Camera = CHelpers::GetComponent<UCameraComponent>(InOwner);
	}

	// ��� ���� Ÿ�Ӷ���
	{
		FOnTimelineVector timeline;
		timeline.BindUFunction(this, "OnSoaring");
		Timeline.AddInterpVector(Curve, timeline);

		FOnTimelineEvent finished;
		finished.BindUFunction(this, "OnTimelimeFinished");
		Timeline.SetTimelineFinishedFunc(finished);
		Timeline.SetPlayRate(SoarSpeed);
	}

	// ī�޶�� Ÿ�Ӷ���
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

	// �����س��� ī�޶��� ���溤�ͷ� ���ư���. 
	FVector direction = CameraRotator.Vector(); // �Ǵ� CameraRotator.GetForwardVector();
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

	// Ÿ�̸� �ڵ� �ʱ�ȭ 
	{
		// �ش� Ÿ�̸� �ڵ� ��� ���� 
		HoldTimerHandle.Invalidate();

		LandCheckTimerHandle.Invalidate();
	}

	// ī�޶� �����Ÿ��� �մ��� 
	SetCameraData();

	DoActionDatas[Index].Begin_Casting(OwnerCharacter);
}



void UCSwordSkill_DragonFall::End_Casting()
{
	DoActionDatas[Index].End_Casting(OwnerCharacter, true);

	// �����ߴ��� �˻� 
	OwnerCharacter->GetWorld()->GetTimerManager().SetTimer(LandCheckTimerHandle,
		this, &UCSwordSkill_DragonFall::OnCheckIfLand, 0.1f, false);
	if (OnSoaringEnd.IsBound())
		OnSoaringEnd.Broadcast();

	StartNextPhase();
}



void UCSwordSkill_DragonFall::Input_AnyKey()
{
	// ī�޶� ������ �����Ѵ�. 
	CheckNull(OwnerCharacter); 
	APlayerController* controller = OwnerCharacter->GetController<APlayerController>();
	CheckNull(controller); 
	
	controller->GetPlayerViewPoint(CameraLocation, CameraRotator);

	ForwardVector = CameraRotator.Vector(); 
	bClickedKey = true; 
	
	// �θ� ���� ȣ�� 
	Super::Input_AnyKey(); 
}


void UCSwordSkill_DragonFall::OffSkillCasting()
{
	CLog::Print("Off Skill Casting Call Complete");

	// ���⼭ ���� �������	
	OnSoarCharacter();
}

void UCSwordSkill_DragonFall::OnSkillDoAction()
{
	// �ԷµǾ� �� ī�޶� �������� ĳ���͸� �����Ѵ�.
	OnDescent();
}

void UCSwordSkill_DragonFall::OffSkillDoAction()
{

}

void UCSwordSkill_DragonFall::RestoreGravity()
{
	if (OwnerCharacter)
	{
		OwnerCharacter->GetCharacterMovement()->GravityScale = 1.0f; // �߷� ����
	}


}

void UCSwordSkill_DragonFall::OnSoaring(FVector Output)
{
	CheckNull(OwnerCharacter); 
	
	//CLog::Log("OutPutZ : " + OwnerCharacter->GetName());
	OriginVector.Z = Output.Z;
	OwnerCharacter->SetActorLocation(OriginVector);
	
	// �߷� ���ֱ�
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
		// ���⼭�� �߷� ���� ���Ѻ�
		RestoreGravity();
		// �����ϸ� ���� ������ 
		CLog::Print("Skill - Landed Complete");

		// �����ϸ� Ű�� ���������� ���� �ִϸ��̼� ���� �ٸ���
		// �׳� ������ �ʰ� �������ٸ� ���� ��Ÿ�� ���� ��ŸƮ 
		if (bClickedKey == false)
		{
			//TODO: ���� ������ �����Ϳ� �����ϰ� �װ� �ҷ����� 
			DoActionDatas[Index].PlaySection_SkillCastingMontage(OwnerCharacter, 1.0f, "EndCasting");

			// �׳� ���������� ��ų  ������ ������� ������. 
			StartNextPhase(); 
		}
		// ����Ű�� ������ ���·� ���� �����ϸ� �� �ִϸ��̼�
		else
		{
			//TODO: ��ų ��� ������ ������ �� ���뵵 ������ �Լ��� �׷��ɷ�..?
			// �Ҹ� ��� 
			DoActionDatas[Index].PlaySoundWave(OwnerCharacter);
			
			// ����� ����
			Create_Collision(); 

			// ���⼱ ���� ����� ������ �� ����� ������ ��ų ����ó���ؾ��Ѵ�. 
			DoActionDatas[Index].PlaySecion_SkillActionMontage(OwnerCharacter, 1.0f, "EndSkill");
		}

		// ī�޶� ���� 
		ResetCameraData(); 

		LandCheckTimerHandle.Invalidate(); 
	}
	else
	{
		// �������� ������ ��� üũ 
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

	// ���� �� ���� 
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
