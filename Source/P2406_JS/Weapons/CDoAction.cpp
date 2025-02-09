#include "Weapons/CDoAction.h"
#include "Global.h"
#include "CAttachment.h"
#include "GameFramework/Character.h"

#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"

#include "GenericTeamAgentInterface.h"

UCDoAction::UCDoAction()
{

}

void UCDoAction::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCEquipment* InEquipment, const TArray<FDoActionData>& InDoActionDatas, const TArray<FHitData>& InHitDatas)
{
	OwnerCharacter = InOwner;
	World = OwnerCharacter->GetWorld();

	State = FHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	Movement = FHelpers::GetComponent<UCMovementComponent>(OwnerCharacter);

	DoActionDatas = InDoActionDatas;
	HitDatas = InHitDatas;
}

void UCDoAction::DoAction()
{
	bInAction = true;

	State->SetActionMode();
}

void UCDoAction::Begin_DoAction()
{
	bBeginAction = true;
}

void UCDoAction::End_DoAction()
{
	bBeginAction = false;

	bInAction = false;
	State->SetIdleMode();

	Movement->Move();
}

ACharacter* UCDoAction::FindBestTarget()
{

	float angle = -2.0f;
	ACharacter* candidate = nullptr;
	for (ACharacter* hitted : Hitted)
	{
		FVector direction = hitted->GetActorLocation() - OwnerCharacter->GetActorLocation();
		direction = direction.GetSafeNormal2D();

		FVector forward = OwnerCharacter->GetActorForwardVector();

		float dot = FVector::DotProduct(direction, forward);
		if (dot < 0.75f || dot < angle)
			continue;

		angle = dot;
		candidate = hitted;
		hitted->Tags.Remove(FName("HitByWeapon"));
	}


	return candidate;
}

bool UCDoAction::IsMyTeam(ACharacter* InAttacker, ACharacter* InOther)
{
	CheckNullResult(InAttacker, false);
	CheckNullResult(InOther, false);

	// Team id 가져와서 자기랑 같으면 넘김 
	IGenericTeamAgentInterface* agent = Cast<IGenericTeamAgentInterface>(InAttacker);
	CheckNullResult(agent, false);

	ETeamAttitude::Type type = agent->GetTeamAttitudeTowards(*InOther);

	if (type == ETeamAttitude::Friendly)
		return true;

	return false;
}

bool UCDoAction::IsOtherIsMe(ACharacter* InOther)
{
	for (ACharacter* hitted : Hitted)
		CheckTrueResult(hitted == InOther, true);

	return false;
}
