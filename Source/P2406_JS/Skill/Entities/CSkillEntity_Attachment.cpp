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

    // 엔티티를 특정한 메쉬에 붙인다.
	FHelpers::AttachTo(this, OwnerCharacter->GetMesh(), OwnerCharacter->GetMesh()->GetBoneName(0), EAttachmentRule::SnapToTarget);
	CheckNull(SkillCollision);


	for (FName socketName : SocketNames)
	{   
        // 소켓의 위치 및 회전 가져오기
        FTransform SocketTransform = skeletal->GetSocketTransform(socketName);

        // SkillCollision과 동일한 타입의 새 컴포넌트 생성
        UCSkillCollisionComponent* newSkillCollision = NewObject<UCSkillCollisionComponent>(this, SkillCollision->GetClass(), socketName);
        if (newSkillCollision)
        {
            newSkillCollision->SetDrawDebug(SkillCollision->GetDrawDebug());
            // 컴포넌트 등록
            newSkillCollision->RegisterComponent();
            newSkillCollision->SetCollisionData(OwnerCharacter, SkillCollision->GetCollisionData(), this);
            newSkillCollision->AttachToComponent(skeletal,
            FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true),
            socketName);
            CollisionTable.Add(socketName, newSkillCollision);
        }
	}
}
