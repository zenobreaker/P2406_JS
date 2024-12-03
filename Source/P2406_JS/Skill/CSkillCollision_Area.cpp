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

	// �浹 Ȱ��ȭ ���� (��: �ݸ��� �ѱ�)
	//SetActorEnableCollision(true);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	// �浹 ��� 
	ApplyCollisionEffect();

	/// ���� �ð� �� ��Ȱ��ȭ
	GetWorld()->GetTimerManager().SetTimer(
		CollisionTimerHandle, this, &ACSkillCollision_Area::DeactivateCollision, HitDatas[Index].CollisionInterval, false);
}

void ACSkillCollision_Area::DeactivateCollision()
{
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// �浹 ��Ȱ��ȭ
	//SetActorEnableCollision(false);

	Index++;

	if (Index < HitDatas.Num())
	{
		ActivateCollision();
	}
	else
	{
		// ��� �浹 ������ ó���� ���� �� ����
		Index = 0;
		Hitted.Num();
		//OnCollisionComplete(); // �ʿ��� ��� �ݹ� ȣ��
	}
}

