#pragma once

#include "CoreMinimal.h"
#include "Characters/CEnemy_AI.h"
#include "CBoss_AI.generated.h"


UCLASS()
class P2406_JS_API ACBoss_AI : public ACEnemy_AI
{
	GENERATED_BODY()

public:
	ACBoss_AI();

protected:
	UPROPERTY(VisibleAnywhere)
	class UCPatternComponent* Pattern;
};
