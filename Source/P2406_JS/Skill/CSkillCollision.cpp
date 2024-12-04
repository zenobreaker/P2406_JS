#include "Skill/CSkillCollision.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Characters/IDamagable.h"

ACSkillCollision::ACSkillCollision()
{
	// 기본 충돌 컴포넌트는 nullptr로 설정, 파생 클래스에서 초기화
	CollisionComponent = nullptr;
}

void ACSkillCollision::BeginPlay()
{
	Super::BeginPlay();

	CLog::Print("Begin Effect");

	ActivateCollision();
}

void ACSkillCollision::SetSkillOwnerData(ACharacter* InOwner,
	const TArray<FSkillHitData>& InHitDatas)
{
	OwnerCharacter = InOwner;
	HitDatas = InHitDatas;

	CLog::Print("Set Skill Data", -1, 10.0f, FColor::Green);
}


void ACSkillCollision::ApplyCollisionEffect()
{

}

void ACSkillCollision::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckNull(OtherActor);

	for (AActor* actor : Ignores)
		CheckTrue(actor == OtherActor);

	for (AActor* hitted : Hitted)
		CheckTrue(hitted == OtherActor);

	Hitted.AddUnique(OtherActor);

	CheckTrue(HitDatas.Num() - 1 < Index);

	CLog::Print("Hit Object" + OtherActor->GetName());

	ACharacter* character = Cast<ACharacter>(OtherActor);
	CheckNull(character);
	
	if (character->Implements<UIDamagable>())
	{
		HitDatas[Index].SendDamage(OwnerCharacter, this, character);
	}

}

void ACSkillCollision::HandleCollision(AActor* HitActor)
{
	CLog::Print("Collision Target!! ", -1, 10.0f, FColor::Red);
}

void ACSkillCollision::DestroyProcess()
{
	if (this)
		Destroy();
}

