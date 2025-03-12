#include "Skill/CSkillEntity.h"
#include "Global.h"
#include "GameFramework/Character.h"

#include "Characters/IDamagable.h"
#include "Characters/CBaseCharacter.h"

#include "Skill/CSkillCollisionComponent.h"


ACSkillEntity::ACSkillEntity()
{
	// �⺻ SceneComponent�� ��Ʈ ������Ʈ�� ����
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
}

void ACSkillEntity::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	FLog::Log("Create Success Skill Entity");
}

void ACSkillEntity::BeginPlay()
{
	Super::BeginPlay();
	CheckNull(OwnerCharacter); 

	/*SetActorLocation(OwnerCharacter->GetActorLocation());*/
}

void ACSkillEntity::EndPlay(const EEndPlayReason::Type EndPlayReason)
{

}

int32 ACSkillEntity::GetDamagedCount()
{
	CheckNullResult(SkillCollision, 0);

	return SkillCollision->GetDamagedCount();
}

void ACSkillEntity::SetSkillEntityData(FSkillCollisionData InData)
{
	//FLog::Log("Entity Creator");
	CreateCollisionByType(InData);
}


void ACSkillEntity::DestroySkill()
{
	SkillCollision->DestroyComponent(); 

	FLog::Log("Skill Destroyed");
	//TODO: ���߿� ������Ʈ �ݸ��ص��ɵ�
	Destroy();
}

void ACSkillEntity::CreateCollisionByType(FSkillCollisionData InData)
{
	if (InData.SkillCollisionClass != nullptr)
	{
		SkillCollision = NewObject<UCSkillCollisionComponent>(this, InData.SkillCollisionClass);
	}

	if (SkillCollision != nullptr)
	{
		// �ش� Ŭ�������� ���ǵ� �Ӽ��� �����Ϸ��� �ν��Ͻ��� �ʿ��ϴ�. 
		if(UClass* skillCollisionClass = InData.SkillCollisionClass.Get())
		{
			bool bDrawDebug = skillCollisionClass->GetDefaultObject<UCSkillCollisionComponent>()->GetDrawDebug();
			SkillCollision->SetDrawDebug(bDrawDebug);
		}
		SkillCollision->RegisterComponent();
		SkillCollision->SetWorldLocationAndRotation(GetActorLocation(),GetActorRotation());
		SkillCollision->SetCollisionData(OwnerCharacter, InData, this);
	}
}

void ACSkillEntity::ActivateCollision(FName InName)
{
	if (InName != NAME_None
		&& CollisionTable.Num() > 0
		&& CollisionTable.Contains(InName))
	{
		CollisionTable[InName]->ActivateCollision(Index++);
		return;
	}

	// �⺻���� ������ ���� ǻ�� ���� �Լ� ȣ��

	CheckNull(SkillCollision);

	SkillCollision->ActivateCollision(Index++);
}


void ACSkillEntity::DeactivateCollision(FName InName)
{
	if (InName != NAME_None
		&& CollisionTable.Num() > 0
		&& CollisionTable.Contains(InName))
	{
		CollisionTable[InName]->DeactivateCollision(Index);
		return;
	}

	CheckNull(SkillCollision);

	SkillCollision->DeactivateCollision(Index);
}

void ACSkillEntity::SetSkillDamageEvent(TArray<TFunction<void()>> InFuncs)
{
	CheckNull(SkillCollision);

	for (auto& Func : InFuncs)
	{
		FOnSkillDamaged NewDelegate;
		NewDelegate.BindLambda(Func);
		SkillCollision->OnSkillDamageds.Add(NewDelegate);
	}
}

void ACSkillEntity::SetSkillDamageEventOneParam(TArray<TFunction<void(ACharacter*)>> InFuncs)
{
	CheckNull(SkillCollision);

	for (auto& Func : InFuncs)
	{
		FOnSkillDamagedOneParam NewDelegate;
		NewDelegate.BindLambda(Func);
		SkillCollision->OnSkillDamagedOneParams.Add(NewDelegate);
	}
}

void ACSkillEntity::SetSkillDamageEventThreeParams(TArray<TFunction<void(ACharacter*, AActor*, ACharacter*)>> InFuncs)
{
	CheckNull(SkillCollision);

	for (auto& Func : InFuncs)
	{
		FOnSkillDamagedThreeParams NewDelegate;
		NewDelegate.BindLambda(Func);
		SkillCollision->OnSkillDamagedThreeParams.Add(NewDelegate);
	}
}

//-----------------------------------------------------------------------------
AActor* ACSkillEntity::GetDamageSource() const
{
	return nullptr;
}
