#pragma once

#include "CoreMinimal.h"
#include "CGuardStructures.h"
#include "UObject/NoExportTypes.h"
#include "CDoParry.generated.h"

UCLASS()
class P2406_JS_API UCDoParry : public UObject
{
	GENERATED_BODY()
	

public:
	
	FORCEINLINE bool GetParring() const { return bParring; }

public:
	UCDoParry();

public:
	void BeginPlay(class ACharacter* InOwnerCharacter, const FParryData& InParryData);

public:
	void DoAction_Parry();

	void End_DoAction_Parry(); 

public:
	void OnHandledTrace
	(
		class ACharacter* InAttacker, 
		class AActor* InAttackCauser, 
		class AActor* InOther
	);
		
	void EndTrace();

private:
	class ACharacter* OwnerCharacter;

	FParryData ParryData; 

private:
	bool bParring = false;
	
	TArray<class ACharacter*> Hits;
};
