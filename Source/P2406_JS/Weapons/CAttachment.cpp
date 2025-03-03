#include "Weapons/CAttachment.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/SceneComponent.h"
#include "Components/ShapeComponent.h"

ACAttachment::ACAttachment()
{
	FHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	Mesh = FHelpers::GetComponent<UPrimitiveComponent>(this);
}

void ACAttachment::BeginPlay()
{
	// 블루프린트에서 BeginPlay가 먼저 호출되므로 그 전에 먼저 세팅해야한다. 
	OwnerCharacter = Cast<ACharacter>(GetOwner());

	TArray<USceneComponent*> children;
	Root->GetChildrenComponents(true, children);

	// 해당 오브젝트의 자식들 중 shape 컴포넌트를 찾아서 델리게이션 할당
	for (USceneComponent* child : children)
	{
		UShapeComponent* shape = Cast<UShapeComponent>(child);

		if (!!shape)
		{
			shape->OnComponentBeginOverlap.AddDynamic(this, &ACAttachment::OnComponentBeginOverlap);
			shape->OnComponentEndOverlap.AddDynamic(this, &ACAttachment::OnComponentEndOverlap);

			Collisions.Add(shape);
		}
	}


	// 실행 시, 충돌체 끄도록하여 처음에 충돌되지 않도록.
	OffCollisions();

	// 부모 처리보다 먼저 처리한다. 
	Super::BeginPlay();
}


void ACAttachment::AttachTo(FName InSocketName)
{
	if (OwnerCharacter->GetMesh()->DoesSocketExist(InSocketName) == false)
	{
		FString string = "Socket is Null " + InSocketName.ToString() + "With " + this->GetName();
		FLog::Log(string);
	}

	AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), InSocketName);
}

void ACAttachment::AttachToCollision(FName InCollisionName)
{
	for (UShapeComponent* collision : Collisions)
	{
		if (collision->GetName() == InCollisionName.ToString())
		{
			collision->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), InCollisionName);

			return;
		}
	}
}



void ACAttachment::OnCollisions()
{
	if (OnAttachmentBeginCollision.IsBound())
		OnAttachmentBeginCollision.Broadcast();

	for (UShapeComponent* shape : Collisions)
		shape->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	bEnable = true; 
}

void ACAttachment::OffCollisions()
{
	if (OnAttachmentEndCollision.IsBound())
		OnAttachmentEndCollision.Broadcast();

	for (UShapeComponent* shape : Collisions)
		shape->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bEnable = false;
}

void ACAttachment::HandleAttachmentOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther)
{
	DYNAMIC_EVENT_CALL_THREE_PARAMS(OnAttachmentBeginOverlap, InAttacker, InAttackCauser, InOther);
	/*if (OnAttachmentBeginOverlap.IsBound())
		OnAttachmentBeginOverlap.Broadcast(OwnerCharacter, this, InOther);*/
}



void ACAttachment::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(OwnerCharacter == OtherActor);

	ACharacter* HitCharacter = Cast<ACharacter>(OtherActor);
	if (HitCharacter)
		HandleAttachmentOverlap(OwnerCharacter, this, HitCharacter);
}

void ACAttachment::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckTrue(OwnerCharacter == OtherActor);

	if (OnAttachmentEndOverlap.IsBound())
		OnAttachmentEndOverlap.Broadcast(OwnerCharacter, Cast<ACharacter>(OtherActor));
}

void ACAttachment::OnDestroy()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	Destroy();
}

bool ACAttachment::CanAttack() const
{
	return bAttackable;
}

bool ACAttachment::IsEnable() const
{
	return bEnable;
}

AActor* ACAttachment::GetDamageSource() const
{
	return OwnerCharacter;
}

