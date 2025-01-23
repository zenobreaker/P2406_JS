#include "Skill/CSkillCollision.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Characters/IDamagable.h"

#include "Characters/CBaseCharacter.h"

ACSkillCollision::ACSkillCollision()
{
	// 기본 충돌 컴포넌트는 nullptr로 설정, 파생 클래스에서 초기화
	CollisionComponent = nullptr;
}

void ACSkillCollision::BeginPlay()
{
	Super::BeginPlay();

//	FLog::Print("Begin Effect");

	ActivateCollision();
}

void ACSkillCollision::SetSkillOwnerData(ACharacter* InOwner,
	const TArray<FSkillHitData>& InHitDatas)
{
	OwnerCharacter = InOwner;
	HitDatas = InHitDatas;

//	FLog::Print("Set Skill Data", -1, 10.0f, FColor::Green);
}


void ACSkillCollision::ApplyCollisionEffect()
{

}

void ACSkillCollision::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckNull(OtherActor);

	//TODO: 팀킬 여부는 언젠가..
	// SetIngoreTeam

	for (AActor* actor : Ignores)
		CheckTrue(actor == OtherActor);

	for (AActor* hitted : Hitted)
		CheckTrue(hitted == OtherActor);


	ACBaseCharacter* character = Cast<ACBaseCharacter>(OtherActor);
	CheckNull(character);

	CheckTrue(CheckMyTeam(OtherActor));

	Hitted.AddUnique(OtherActor);

	CheckTrue(HitDatas.Num() - 1 < Index);

	//FLog::Print("Hit Object" + OtherActor->GetName());



	if (character->Implements<UIDamagable>())
	{
		HitDatas[Index].SendDamage(OwnerCharacter, this, character);
	}

}

void ACSkillCollision::HandleCollision(AActor* HitActor)
{
	//FLog::Print("Collision Target!! ", -1, 10.0f, FColor::Red);
}

void ACSkillCollision::DestroyProcess()
{
	if (this)
		Destroy();
}

bool ACSkillCollision::CheckMyTeam(AActor* InOtherActor)
{

	ACBaseCharacter* character = Cast<ACBaseCharacter>(InOtherActor);
	CheckNullResult(character, false);
	auto OtherTeamID = character->GetGenericTeamId();

	ACBaseCharacter* myBase = Cast< ACBaseCharacter >(OwnerCharacter);
	CheckNullResult(myBase, false);

	auto myTeamid = myBase->GetGenericTeamId();

	CheckTrueResult(FGenericTeamId::GetAttitude(OtherTeamID, myTeamid) == ETeamAttitude::Hostile, false);

	return true;
}

