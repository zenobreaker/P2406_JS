#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "CBTDecorator_CprFloatLessEqaul.generated.h"


UCLASS()
class P2406_JS_API UCBTDecorator_CprFloatLessEqaul : public UBTDecorator
{
	GENERATED_BODY()

public:
	UCBTDecorator_CprFloatLessEqaul(); 

private:
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector A; 

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector B; 

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual FString GetStaticDescription() const override;
};
