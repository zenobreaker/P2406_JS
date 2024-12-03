#include "Skill/CSkillCollision_Area.h"
#include "Global.h"
#include "Components/SphereComponent.h"

ACSkillCollision_Area::ACSkillCollision_Area()
{
	CHelpers::CreateComponent<USphereComponent>(this, &AreaCollisionComponent, "Area");

	CollisionComponent = AreaCollisionComponent;
	Index = 0; 

	if (!!CollisionComponent)
	{
		//CLog::Print("Set Overlap", -1, 10.0f, FColor::Green);
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this,
			&ACSkillCollision_Area::OnComponentBeginOverlap);
	}
}

void ACSkillCollision_Area::ActivateCollision()
{
	CheckTrue(HitDatas.Num() <= Index);
	CheckNull(CollisionComponent);

	// 충돌 활성화 로직 (예: 콜리전 켜기)
	//SetActorEnableCollision(true);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	// 충돌 기능 
	ApplyCollisionEffect();

	/// 일정 시간 후 비활성화
	GetWorld()->GetTimerManager().SetTimer(
		CollisionTimerHandle, this, &ACSkillCollision_Area::DeactivateCollision, HitDatas[Index].CollisionInterval, false);
}

void ACSkillCollision_Area::DeactivateCollision()
{
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// 충돌 비활성화
	//SetActorEnableCollision(false);

	Index++;

	if (Index < HitDatas.Num())
	{
		ActivateCollision();
	}
	else
	{
		// 모든 충돌 데이터 처리가 끝난 후 리셋
		Index = 0;
		Hitted.Num();
		//OnCollisionComplete(); // 필요한 경우 콜백 호출
	}
}

