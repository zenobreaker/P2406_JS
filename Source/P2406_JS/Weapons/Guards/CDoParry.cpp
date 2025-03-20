#include "CDoParry.h"
#include "Global.h"

UCDoParry::UCDoParry()
{

}

void UCDoParry::BeginPlay(ACharacter* InOwnerCharacter, const FParryData& InParryData)
{
	OwnerCharacter = InOwnerCharacter; 

	ParryData = InParryData;
}

void UCDoParry::DoAction_Parry()
{

	bParring = true;

	ParryData.DoAction_Parry(OwnerCharacter);
}

void UCDoParry::End_DoAction_Parry()
{
	bParring = false; 
}

void UCDoParry::OnHandledTrace(ACharacter* InAttacker, AActor* InAttackCauser, AActor* InOther)
{
	CheckNull(InAttacker);

	CheckFalse(bParring);

	ParryData.HitData.SendDamage(InAttacker, InAttackCauser, InOther, Hits.Num() <= 1);
}

void UCDoParry::EndTrace()
{
	Hits.Empty();
}
