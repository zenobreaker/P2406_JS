#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CGuardData.generated.h"

UCLASS()
class P2406_JS_API UCGuardData : public UObject
{
	GENERATED_BODY()
	
private:
	friend class UCGuardDataAsset; 

public:
	~UCGuardData();
public:
	FORCEINLINE class UCDoGuard* GetGuard() { return Guard; }
	FORCEINLINE class UCDoParry* GetParry() { return Parry; }

private: 
	UPROPERTY() 
	class UCDoGuard* Guard = nullptr; 

	UPROPERTY() 
	class UCDoParry* Parry = nullptr;
};
