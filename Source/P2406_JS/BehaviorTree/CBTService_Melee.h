#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_Melee.generated.h"


UCLASS()
class P2406_JS_API UCBTService_Melee : public UBTService
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Action")
	float ActionRange = 150;

	UPROPERTY(EditAnywhere, Category = "Action")
	FVector2D ActionDelay = FVector2D::TVector2(1.5f, 2.0f);

public:
	UCBTService_Melee();

protected:
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	void RadnomActionDelay();

private:
	float CurrentDelay;
};
