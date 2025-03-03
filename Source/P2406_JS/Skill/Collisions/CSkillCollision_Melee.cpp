#include "CSkillCollision_Melee.h"
#include "Global.h"

#include "Components/SphereComponent.h"

UCSkillCollision_Melee::UCSkillCollision_Melee()
{
	
	
}

void UCSkillCollision_Melee::BeginPlay()
{
	Super::BeginPlay();

}

void UCSkillCollision_Melee::SetCollision(UPrimitiveComponent* InComponent)
{

}

void UCSkillCollision_Melee::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(OwnerCharacter == OtherActor);


}

void UCSkillCollision_Melee::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckTrue(OwnerCharacter == OtherActor);


}

//void UCSkillCollision_Melee::ActivateCollision()
//{
//
//}
//
//void UCSkillCollision_Melee::DeactivateCollision()
//{
//
//}
