#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CAttackTraceComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHandledTrace, class ACharacter*, InAttacker,class AActor*, InAttackCauser, class ACharacter*, InOther);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndTrace);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class P2406_JS_API UCAttackTraceComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	  // 트레이스 설정
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Trace")
    float TraceRadius = 50.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Trace")
    FName StartSocketName = "TraceStart";

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Trace")
    FName EndSocketName = "TraceEnd";

public:	
	UCAttackTraceComponent();

protected:
	virtual void BeginPlay() override;

    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
public:
    
    void SetTrace(); 
    void SetEndTrace();
    // 트레이스 실행 함수
    //void PerformTrace(AActor* OwningWeapon, TArray<FHitResult>& OutHits);

    void HandleTrace(AActor* InHitActor);

public:
	UFUNCTION()
    void OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType);

private:
    bool GetMyTeam(class AActor* InHitTarget);

public:
    FOnHandledTrace OnHandledTrace;
    FOnEndTrace OnEndTrace; 

private:
    bool bIsAttacking = false; 
    FVector StartVec; 
    FVector EndVec; 

private:
    TArray<class ACharacter*> Hits; 
    class ACharacter* OwnerCharacter; 
    class UCWeaponComponent* Weapon; 
    class ACAttachment* Attachment;
};
