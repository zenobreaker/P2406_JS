#include "CGuardDataAsset.h"
#include "Global.h"
#include "CDoGuard.h"
#include "CDoParry.h"
#include "CGuardData.h"

UCGuardDataAsset::UCGuardDataAsset()
{
	DoGuardClass = UCDoGuard::StaticClass();
	DoParryClass = UCDoParry::StaticClass();
}

void UCGuardDataAsset::BeginPlay(ACharacter* InOwnerCharacter, UCGuardData** OutGuardData)
{

	UCDoGuard* doGuard = nullptr;
	if (!!DoGuardClass)
	{
		doGuard = NewObject<UCDoGuard>(this, DoGuardClass);
		doGuard->BeginPlay(InOwnerCharacter, GuardData, GuardHitData, CounterData);
	}

	UCDoParry* doParry = nullptr;
	if (!!DoParryClass)
	{
		doParry = NewObject<UCDoParry>(this, DoParryClass); 
		doParry->BeginPlay(InOwnerCharacter, ParryData);
	}

	*OutGuardData = NewObject<UCGuardData>();
	(*OutGuardData)->Guard = doGuard;
	(*OutGuardData)->Parry= doParry;
}
