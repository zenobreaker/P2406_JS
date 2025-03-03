#include "Components/CAttackTraceComponent.h"
#include "Global.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

#include "Characters/CPlayer.h"
#include "Components/CWeaponComponent.h"
#include "Components/CConditionComponent.h"
#include "Weapons/CAttachment.h"


#define  LOG_UCAttackTraceComponent

UCAttackTraceComponent::UCAttackTraceComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCAttackTraceComponent::BeginPlay()
{
	Super::BeginPlay();


	OwnerCharacter = Cast<ACharacter>(GetOwner());
	Player = Cast<ACPlayer>(GetOwner());
	if (!!OwnerCharacter)
		Weapon = FHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter);

	REGISTER_EVENT_WITH_REPLACE(Weapon, OnWeaponTypeChanged, this, UCAttackTraceComponent::OnWeaponTypeChanged);
}

void UCAttackTraceComponent::TickComponent(float DeltaTime,
	ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckFalse(bIsAttacking);
	CheckNull(OwnerCharacter);
	CheckNull(Weapon);
	CheckNull(Weapon->GetAttachment());

	ACAttachment* attachment = Weapon->GetAttachment();
	CheckNull(attachment);

	FHitResult TraceHitResult;
	FCollisionQueryParams QueryParams;

	QueryParams.AddIgnoredActor(OwnerCharacter);
	QueryParams.AddIgnoredActor(attachment);

	// 메시 갖고옴 
	UPrimitiveComponent* mesh =
		FHelpers::GetComponent<UPrimitiveComponent>(Weapon->GetAttachment());
	CheckNull(mesh);

	// 위치 갱신 
	FName goalName = attachment->GetSocketGoalName();
	FName startName = attachment->GetSocketStartName();

	FVector startLocation = mesh->GetSocketLocation(startName);
	FVector endLocation = mesh->GetSocketLocation(goalName);

	FVector direction = (endLocation - startLocation).GetSafeNormal();
	FQuat quat = FRotationMatrix::MakeFromX(direction).ToQuat();
	// X 축으로 긴 캡슐 
	quat = quat * FQuat(FVector(0, 1, 0), FMath::DegreesToRadians(90.0f));

	// 캡슐 트레이스 실행
	bool bCheck = GetWorld()->SweepSingleByChannel(
		TraceHitResult,
		startLocation,
		endLocation,
		quat,
		ECC_Pawn,  // 충돌 채널
		FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight),
		QueryParams
	);


#ifdef  LOG_UCAttackTraceComponent
	FColor color = bCheck ? FColor::Red : FColor::Blue;

	DrawDebugLine(GetWorld(), startLocation, endLocation, color, false, 1);
	DrawDebugCapsule(GetWorld(),
		(endLocation + startLocation) * 0.5f,
		CapsuleHalfHeight,
		CapsuleRadius, quat, color, false, 1);
#endif //  LOG_UCAttackTraceComponent


	if (bCheck)
	{
		for (ACharacter* hit : Hits)
			CheckTrue(hit == TraceHitResult.GetActor());

		TraceHitResult.GetActor()->Tags.Add(FName("NormalTrace"));
		HandleTrace(TraceHitResult.GetActor());
		
		return; 
	}
	
	HandleAirborneTrace();
}

void UCAttackTraceComponent::SetBeginTrace()
{
	//StartVec = InStart;
	//EndVec = InEnd; 

	bIsAttacking = true;
}

void UCAttackTraceComponent::SetEndTrace()
{
	CheckFalse(bIsAttacking);
	bIsAttacking = false;

	Hits.Empty();

	DYNAMIC_EVENT_CALL(OnEndTrace);
}

void UCAttackTraceComponent::HandleTrace(AActor* InHitActor)
{
	CheckNull(InHitActor);
	// 간단한 충돌 처리
	// 
	 // 캐릭터 여부 확인
	ACharacter* hitCharacter = Cast<ACharacter>(InHitActor);
	CheckNull(hitCharacter);

	// 아군 검사 
	bool bFriend = GetMyTeam(InHitActor);
	if (bFriend)
		return;

	// 이미 무기로 충돌 처리된 경우 무시
	if (hitCharacter->Tags.Contains(FName("HitByWeapon")))
		return;

	// 트레이스 처리
	Hits.AddUnique(hitCharacter);
	//UE_LOG(LogTemp, Log, TEXT("Trace Hit Actor: %s"), *InHitActor->GetName());

	CheckNull(Weapon->GetAttachment());

	Hitcount++; 
	switch (CurrentType)
	{
		case EAttackType::NormalAttack:
			DYNAMIC_EVENT_CALL_THREE_PARAMS(
				OnHandledTrace, OwnerCharacter, Weapon->GetAttachment(), hitCharacter);
			break;
		case EAttackType::ParryAttack:
			DYNAMIC_EVENT_CALL_THREE_PARAMS(
				OnHandledParryTrace, OwnerCharacter, Weapon->GetAttachment(), hitCharacter);
			break;
		case EAttackType::JumpAttack:
			DYNAMIC_EVENT_CALL_THREE_PARAMS(
				OnHandledJumpTrace, OwnerCharacter, Weapon->GetAttachment(), hitCharacter);
			break;
		default:
			break;
	}


}

bool UCAttackTraceComponent::HandleAirborneTrace()
{
	CheckNullResult(Weapon, false);
	CheckNullResult(Weapon->GetAttachment(), false);


	//FHitResult TraceHitResult;
	TArray<FHitResult> traceHitResults;
	FCollisionQueryParams QueryParams;
	ACAttachment* attachment = Weapon->GetAttachment();
	CheckNullResult(attachment, false);

	QueryParams.AddIgnoredActor(OwnerCharacter);
	QueryParams.AddIgnoredActor(attachment);

	float radius = 200.0f;
	
	if (!!Player && Player->IsJumping())
		radius = 250.0f;

	// 위치 갱신 
	FVector startLocation = OwnerCharacter->GetActorLocation();
	FVector endLocation = startLocation - OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();;

	// 트레이스 실행
	bool bCheck = GetWorld()->SweepMultiByObjectType(
		traceHitResults,
		startLocation,
		endLocation,
		FQuat::Identity,
		ECC_Pawn,  // 충돌 채널
		FCollisionShape::MakeSphere(radius),
		QueryParams
	);



#ifdef  LOG_UCAttackTraceComponent
	FColor color = bCheck ? FColor::Orange : FColor::Green;

	DrawDebugSphere(GetWorld(), startLocation, radius, 10, color, false, 3);
#endif //  LOG_UCAttackTraceComponent


	if (bCheck)
	{
		for (FHitResult& hit : traceHitResults)
		{
			AActor* HitActor = hit.GetActor();
			
			UCConditionComponent* targetCondition = FHelpers::GetComponent<UCConditionComponent>(hit.GetActor());
			if (targetCondition == nullptr)
				continue; 

			if (targetCondition->GetAirborneCondition() == false)
				continue; 


			if (HitActor->Tags.Contains(FName("NormalTrace")))
				return false;
			
			if (Hits.Contains(HitActor))
				continue;

			HandleTrace(hit.GetActor());
		}
	}

	return false;
}

//-----------------------------------------------------------------------------

void UCAttackTraceComponent::OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType)
{
	SetEndTrace();
}

void UCAttackTraceComponent::OnNormalAttack()
{
	CurrentType = EAttackType::NormalAttack;
}

void UCAttackTraceComponent::OnParryAttack()
{
	CurrentType = EAttackType::ParryAttack;
}

void UCAttackTraceComponent::OnJumpAttack()
{
	CurrentType = EAttackType::JumpAttack;
}

bool UCAttackTraceComponent::GetMyTeam(AActor* InHitTarget)
{
	// Team id 가져와서 자기랑 같으면 넘김 
	IGenericTeamAgentInterface* agent = Cast<IGenericTeamAgentInterface>(OwnerCharacter);
	CheckNullResult(agent, false);

	ETeamAttitude::Type type = agent->GetTeamAttitudeTowards(*InHitTarget);

	if (type == ETeamAttitude::Friendly)
		return true;


	return false;
}

