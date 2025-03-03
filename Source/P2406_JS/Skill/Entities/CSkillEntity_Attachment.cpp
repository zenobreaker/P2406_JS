#include "Skill/Entities/CSkillEntity_Attachment.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Skill/CSkillCollisionComponent.h"

void ACSkillEntity_Attachment::BeginPlay()
{
	Super::BeginPlay();
	CheckNull(OwnerCharacter); 

	USkeletalMeshComponent* skeletal = FHelpers::GetComponent<USkeletalMeshComponent>(OwnerCharacter);
	CheckNull(skeletal);

	FHelpers::AttachTo(this,OwnerCharacter->GetMesh());

	CheckNull(SkillCollision);

	for (FName socketName : SocketNames)
	{
        // ������ ��ġ �� ȸ�� ��������
        FTransform SocketTransform = skeletal->GetSocketTransform(socketName);

        // SkillCollision�� ������ Ÿ���� �� ������Ʈ ����
        UCSkillCollisionComponent* NewSkillCollision = NewObject<UCSkillCollisionComponent>(this, SkillCollision->GetClass(), socketName);
        if (NewSkillCollision)
        {
            // ������Ʈ ���
            NewSkillCollision->RegisterComponent();

            SkillCollisions.Add(NewSkillCollision);
        }
	}
}

void ACSkillEntity_Attachment::ActivateCollision(FName InName)
{
    CheckFalse(SkillCollisions.Num() > 0);

    for (auto& collision : SkillCollisions)
    {
        if (collision->GetName() == InName)
        {
            collision->ActivateCollision();
            
            return;
        }
    }
}

void ACSkillEntity_Attachment::DeactivateCollision(FName InName)
{
    CheckFalse(SkillCollisions.Num() > 0);

    for (auto& collision : SkillCollisions)
    {
        if (collision->GetName() == InName)
        {
            collision->DeactivateCollision();

            return;
        }
    }
}
