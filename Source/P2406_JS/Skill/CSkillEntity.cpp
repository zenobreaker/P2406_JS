#include "Skill/CSkillEntity.h"
#include "Global.h"
#include "GameFramework/Character.h"

#include "Characters/IDamagable.h"
#include "Characters/CBaseCharacter.h"

#include "Skill/CSkillCollisionComponent.h"


ACSkillEntity::ACSkillEntity()
{
	// 기본 SceneComponent를 루트 컴포넌트로 설정
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	CollisionTable.Emplace("Default", TArray<UCSkillCollisionComponent*>());
}

void ACSkillEntity::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	FLog::Log("Create Success Skill Entity");
}

void ACSkillEntity::BeginPlay()
{
	Super::BeginPlay();

}

void ACSkillEntity::EndPlay(const EEndPlayReason::Type EndPlayReason)
{

}

int32 ACSkillEntity::GetDamagedCount()
{
	return DamagedList.Num();
}

void ACSkillEntity::SetSkillEntityData(TArray<FSkillCollisionData>& InDatas)
{
	CheckFalse(InDatas.Num() > 0);

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
			skillCollision->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
			skillCollision->RegisterComponent();
			skillCollision->SetWorldLocationAndRotation(GetActorLocation(), GetActorRotation());
			skillCollision->SetCollisionData(OwnerCharacter, data, this);

			if (data.GroupName.IsNone() || data.GroupName.IsEqual("Default"))
				CollisionTable.FindOrAdd("Default").AddUnique(skillCollision);
			else
				CollisionTable.FindOrAdd(data.GroupName).AddUnique(skillCollision);
		}
	}

}

void ACSkillEntity::DestroySkill()
{
	for (TPair<FName, TArray<UCSkillCollisionComponent*>>& pair : CollisionTable)
	{
		for (UCSkillCollisionComponent* skill : pair.Value)
			skill->DestroyComponent();
	}

	FLog::Log("Skill Destroyed");
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

	Destroy();
}

void ACSkillEntity::OnDamaged(AActor* InOther)
{
	CheckTrue(InOther == OwnerCharacter);

	DamagedList.AddUnique(InOther);
}


void ACSkillEntity::ActivateCollisionSequence(FName InName)
{
	CheckFalse(CollisionTable.Contains(InName));
	CheckFalse(CollisionTable.Num() > 0);

	const TArray<UCSkillCollisionComponent*>& SCCs = CollisionTable[InName];

	bool bIsFirst = true;

	for (UCSkillCollisionComponent* scc : SCCs)
	{
		if (bIsFirst)
		{
			ActivateCollisionComponent(scc);
			bIsFirst = false;
		}
		else
		{
			FTimerDelegate timerDelegate;
			timerDelegate.BindUObject(this, &ACSkillEntity::ActivateCollisionComponent, scc);

			float nextDelay = scc->GetCollisionData().NextDelay;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, timerDelegate, nextDelay, false);
		}

		UpdateSCC(InName, scc);
	}
}

void ACSkillEntity::ActivateCollisionComponent(UCSkillCollisionComponent* InSCC)
{
	CheckNull(InSCC);

	InSCC->ActivateCollision();
}

void ACSkillEntity::ActivateCollision(FName InName)
{
	if (InName == NAME_None)
		InName = "Default";

	ActivateCollisionSequence(InName);
}


void ACSkillEntity::DeactivateCollision(FName InName)
{
	if (CollisionTable.Num() > 0
		&& CollisionTable.Contains(InName))
	{
		for (UCSkillCollisionComponent* skill : CollisionTable[InName])
		{
			if (skill == nullptr)
				continue;
			skill->DeactivateCollision();
		}
		return;
	}
}

//-----------------------------------------------------------------------------
AActor* ACSkillEntity::GetDamageSource() const
{
	return nullptr;
}
