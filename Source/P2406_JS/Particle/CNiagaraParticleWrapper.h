#pragma once

#include "CoreMinimal.h"
#include "Particle/CParticleBase.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "CNiagaraParticleWrapper.generated.h"


UCLASS()
class P2406_JS_API UCNiagaraParticleWrapper : public UCParticleBase
{
	GENERATED_BODY()
	
private:
	UNiagaraSystem* NiagaraSystem;


public:
    virtual void PlayParticle() override;
   
    virtual void StopParticle() override;

};
