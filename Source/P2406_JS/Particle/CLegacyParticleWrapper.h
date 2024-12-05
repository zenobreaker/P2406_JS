#pragma once

#include "CoreMinimal.h"
#include "Particle/CParticleBase.h"
#include "Particles/ParticleSystem.h"
#include "CLegacyParticleWrapper.generated.h"


UCLASS()
class P2406_JS_API UCLegacyParticleWrapper : public UCParticleBase
{
	GENERATED_BODY()
	
private:
	UParticleSystem* LegacyParticleSystem;

public:
	virtual void PlayParticle() override;

	virtual void StopParticle() override;

};
