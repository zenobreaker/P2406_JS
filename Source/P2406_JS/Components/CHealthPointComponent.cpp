#include "Components/CHealthPointComponent.h"


UCHealthPointComponent::UCHealthPointComponent()
{
	
}


void UCHealthPointComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	
}

void UCHealthPointComponent::Damage(float InAmount)
{
	Health += (InAmount * -1.0f);
	Health = FMath::Clamp(Health, 0.0f, MaxHealth);

	if (OnHealthPointChanged.IsBound())
		OnHealthPointChanged.Broadcast(Health, MaxHealth);
}


