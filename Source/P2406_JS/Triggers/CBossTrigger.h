#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CBossTrigger.generated.h"

UCLASS(Blueprintable)
class P2406_JS_API ACBossTrigger : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = "Trigger")
	TSubclassOf<UShapeComponent> TriggerShapeClass;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger")
	UShapeComponent* TriggerShape;

public:	
	ACBossTrigger();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnTriggerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnTriggerExit(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);


private:
	FTimerHandle TimerHandle; 
};
