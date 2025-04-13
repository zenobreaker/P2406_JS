#include "Skill/Entities/CSkillEntity_Attachment.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Skill/CSkillCollisionComponent.h"
#include "Skill/Collisions/CSkillCollision_Shape.h"


void ACSkillEntity_Attachment::BeginPlay()
{
	Super::BeginPlay();
	CheckNull(OwnerCharacter); 
    // 엔티티를 특정한 메쉬에 붙인다.
	FHelpers::AttachTo(this, OwnerCharacter->GetMesh(), OwnerCharacter->GetMesh()->GetBoneName(0), EAttachmentRule::SnapToTarget);
}

void ACSkillEntity_Attachment::Tick(float DeltaSeconds)
{

	
	CheckNull(OwnerCharacter);
	USkeletalMeshComponent* skeletal = FHelpers::GetComponent<USkeletalMeshComponent>(OwnerCharacter);
	CheckNull(skeletal);

}


void ACSkillEntity_Attachment::SetSkillEntityData(TArray<FSkillCollisionData>& InDatas)
{
	CheckFalse(InDatas.Num() > 0);
	CheckNull(OwnerCharacter);

	USkeletalMeshComponent* skeletal = FHelpers::GetComponent<USkeletalMeshComponent>(OwnerCharacter);
	CheckNull(skeletal);

	// 소켓의 위치 및 회전 가져오기
	for (FName& socketName : SocketNames)
	{
		FTransform SocketTransform = skeletal->GetSocketTransform(socketName);

		for (FSkillCollisionData& data : InDatas)
		{
			if (data.SkillCollisionClass == nullptr || data.SkillCollisionClass == (UCSkillCollisionComponent::StaticClass()))
				continue;

			UCSkillCollisionComponent* skillCollision = nullptr;
			skillCollision = NewObject<UCSkillCollisionComponent>(this, data.SkillCollisionClass);

			if (skillCollision != nullptr)
			{
				// 해당 클래스에서 정의된 속성에 접근하려면 인스턴스가 필요하다. 
				bool bDrawDebug = false;
				if (UClass* skillCollisionClass = data.SkillCollisionClass.Get())
				{
					bDrawDebug = skillCollisionClass->GetDefaultObject<UCSkillCollisionComponent>()->GetDrawDebug();
				}
				skillCollision->SetDrawDebug(bDrawDebug);
				skillCollision->AttachToComponent
				(
					skeletal
					, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true)
					, socketName
				);
				skillCollision->RegisterComponent();
				//skillCollision->SetWorldLocationAndRotation(GetActorLocation(), GetActorRotation());
				skillCollision->SetCollisionData(OwnerCharacter, data, this);

				if (data.GroupName.IsEqual(socketName))
					CollisionTable.FindOrAdd(data.GroupName).AddUnique(skillCollision);
			}
		}
	}

	//Super::SetSkillEntityData(InDatas);
}

void ACSkillEntity_Attachment::UpdateSCC(FName InName, UCSkillCollisionComponent* InSCC)
{
	CheckNull(InSCC); 

	UCSkillCollision_Shape* shape   = Cast<UCSkillCollision_Shape>(InSCC);
	CheckNull(shape);

	shape->SetUpdated(true);
	shape->SetTargetName(InName);
}

