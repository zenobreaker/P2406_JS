#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CAirborneComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class P2406_JS_API UCAirborneComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCAirborneComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
