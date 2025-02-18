#include "Skill/CSkillEntity.h"
#include "Global.h"
#include "GameFramework/Character.h"

#include "Characters/IDamagable.h"
#include "Characters/CBaseCharacter.h"

#include "Skill/CSkillCollisionComponent.h"
#include "Skill/Collisions/CSkillCollision_Area.h"

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

ACSkillEntity::ACSkillEntity()
{

}

void ACSkillEntity::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	FLog::Log("Create Success Skill Entity");
}

void ACSkillEntity::BeginPlay()
{
	Super::BeginPlay();


	ActivateCollision();
}

void ACSkillEntity::EndPlay(const EEndPlayReason::Type EndPlayReason)
{

}

void ACSkillEntity::DestroySkill()
{
	SkillCollision->DestroyComponent(); 

	FLog::Log("Skill Destroied");
	//TODO: 나중에 오브젝트 콜링해도될듯
	Destroy();
}

void ACSkillEntity::CreateCollisionByType(FSkillCollisionData InData)
{
	//// 범위형 스킬콜리전 
	//switch (MyType)
	//{
	//	case ESkillCollisionType::Area:
	//	{
	//		SkillCollision = NewObject<UCSkillCollision_Area>(this, "Area");
	//	}
	//	break;
	//}

	if (InData.Collision != nullptr)
	{
		SkillCollision = NewObject<UCSkillCollisionComponent>(this, InData.Collision);
	}


	if (SkillCollision != nullptr)
	{
		SkillCollision->RegisterComponent();
		//SkillCollision->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		SkillCollision->SetCollisionData(OwnerCharacter, InData);
	}
}

void ACSkillEntity::ActivateCollision()
{
	CheckNull(SkillCollision);

	SkillCollision->ActivateCollision();
}

void ACSkillEntity::DeactivateCollision()
{
	CheckNull(SkillCollision);

	SkillCollision->DeactivateCollision();
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
