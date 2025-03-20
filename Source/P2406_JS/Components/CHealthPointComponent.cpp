#include "Components/CHealthPointComponent.h"
#include "Global.h"

UCHealthPointComponent::UCHealthPointComponent()
{
	
}


void UCHealthPointComponent::BeginPlay()
{
	Super::BeginPlay();
	
	AActor* OwnerActor = GetOwner();
	if (OwnerActor != nullptr)
	{
		// 정상적으로 Owner가 있는 경우
		FLog::Log("Owner : " + OwnerActor->GetName());
	}
	else
	{
		// Owner가 nullptr인 경우
		FLog::Log("Not Find Owner");
	}
	Health = MaxHealth;
	
}

void UCHealthPointComponent::OnComponentCreated()
{
	Super::OnComponentCreated();
	RegisterComponent();
}

void UCHealthPointComponent::Damage(float InAmount)
{
	Health += (InAmount * -1.0f);
	Health = FMath::Clamp(Health, 0.0f, MaxHealth);

	if (OnHealthPointChanged.IsBound())
		OnHealthPointChanged.Broadcast(Health, MaxHealth);
}


