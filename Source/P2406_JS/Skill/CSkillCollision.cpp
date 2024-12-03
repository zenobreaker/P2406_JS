#include "Skill/CSkillCollision.h"
#include "Global.h"
#include "GameFramework/Character.h"

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
	CLog::Print("Hit Object");
	CheckNull(OtherActor);

	for (AActor* hitted : Hitted)
		CheckTrue(hitted == OtherActor);

	Hitted.AddUnique(OtherActor);

	CheckTrue(HitDatas.Num() - 1 < Index);
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

