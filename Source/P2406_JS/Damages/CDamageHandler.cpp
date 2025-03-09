#include "Damages/CDamageHandler.h"
#include "Global.h"
#include "Weapons/CWeaponStructures.h"

void UCDamageHandler::BeginPlay(ACharacter* InCharacter)
{
	OwnerCharacter = InCharacter;
}

void UCDamageHandler::ApplyDamage(const FHitData& InHitData)
{
	CheckNull(OwnerCharacter); 
	//TODO : 흠.. 데미지 유형에 따른 피격 모션... 그래... 
	EDamageType type = EDamageType::Max;
	if (InHitData.Airial > 0)
		type = EDamageType::Airbone;
	else if (InHitData.Launch > 0)
		type = EDamageType::Launch;
	else if (InHitData.bDown)
		type = EDamageType::Down; 

	// 피격 당했을 때 유형 타입에 따른 데미지 모션을 수행한다. 
}
