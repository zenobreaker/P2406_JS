#include "Skill/CSkillCollision_Area.h"
#include "Global.h"
#include "Components/SphereComponent.h"

ACSkillCollision_Area::ACSkillCollision_Area()
{
	Index = 0; 
	CHelpers::CreateComponent<USphereComponent>(this, &AreaCollisionComponent, "Area");
	CheckNull(AreaCollisionComponent);


	CollisionComponent = AreaCollisionComponent;

	if (!!CollisionComponent)
	{
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this,
			&ACSkillCollision_Area::OnComponentBeginOverlap);
	}
}

void ACSkillCollision_Area::ActivateCollision()
{
	CheckTrue(HitDatas.Num() <= Index);
	CheckNull(CollisionComponent);

	AreaCollisionComponent->SetSphereRadius(CollisionRadius);
	AreaCollisionComponent->UpdateBounds();
	AreaCollisionComponent->MarkRenderStateDirty();
	CollisionComponent = AreaCollisionComponent;

	CLog::Print("Overlap Activate ", -1, 10.0f, FColor::Green);

	// �浹 Ȱ��ȭ ���� (��: �ݸ��� �ѱ�)
	SetActorEnableCollision(true);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	// �浹 ��� 
	ApplyCollisionEffect();

	if (GetWorld() == nullptr)
		CLog::Log("Skill Calls Get World() is Null");

	/// ���� �ð� �� ��Ȱ��ȭ
	GetWorld()->GetTimerManager().SetTimer(
		CollisionTimerHandle, this, &ACSkillCollision_Area::DeactivateCollision, HitDatas[Index].CollisionInterval, false);
}

void ACSkillCollision_Area::DeactivateCollision()
{
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// �浹 ��Ȱ��ȭ
	SetActorEnableCollision(false);

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
		// �� �̻� �Ұ� ������ ��ų �ݸ��� ���� 

		DestroyProcess();
	}
}

