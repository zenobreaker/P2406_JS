#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Global.h"
#include "CParticleBase.generated.h"


UCLASS()
class P2406_JS_API UCParticleBase : public UObject
{
	GENERATED_BODY()

public:
	virtual void PlayParticle() PURE_VIRTUAL(UCParticleBase::PlayParticle, );
	virtual void StopParticle() PURE_VIRTUAL(UCParticleBase::StopPArticle, );
};
