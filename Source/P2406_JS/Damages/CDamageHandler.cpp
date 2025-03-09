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
	//TODO : ��.. ������ ������ ���� �ǰ� ���... �׷�... 
	EDamageType type = EDamageType::Max;
	if (InHitData.Airial > 0)
		type = EDamageType::Airbone;
	else if (InHitData.Launch > 0)
		type = EDamageType::Launch;
	else if (InHitData.bDown)
		type = EDamageType::Down; 

	// �ǰ� ������ �� ���� Ÿ�Կ� ���� ������ ����� �����Ѵ�. 
}
