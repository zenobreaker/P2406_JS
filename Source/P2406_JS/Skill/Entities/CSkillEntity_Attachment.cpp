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

    // ��ƼƼ�� Ư���� �޽��� ���δ�.
	FHelpers::AttachTo(this, OwnerCharacter->GetMesh(), OwnerCharacter->GetMesh()->GetBoneName(0), EAttachmentRule::SnapToTarget);
	CheckNull(SkillCollision);


	for (FName socketName : SocketNames)
	{   
        // ������ ��ġ �� ȸ�� ��������
        FTransform SocketTransform = skeletal->GetSocketTransform(socketName);

        // SkillCollision�� ������ Ÿ���� �� ������Ʈ ����
        UCSkillCollisionComponent* newSkillCollision = NewObject<UCSkillCollisionComponent>(this, SkillCollision->GetClass(), socketName);
        if (newSkillCollision)
        {
            newSkillCollision->SetDrawDebug(SkillCollision->GetDrawDebug());
            // ������Ʈ ���
            newSkillCollision->RegisterComponent();
            newSkillCollision->SetCollisionData(OwnerCharacter, SkillCollision->GetCollisionData(), this);
            newSkillCollision->AttachToComponent(skeletal,
            FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true),
            socketName);
            CollisionTable.Add(socketName, newSkillCollision);
        }
	}
}
