#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CAttackTraceComponent.generated.h"

// ���� Ÿ���� ����
UENUM(BlueprintType)
enum class EAttackType : uint8
{
    NormalAttack UMETA(DisplayName = "Normal Attack"),
    ParryAttack UMETA(DisplayName = "Parry Attack"),
    JumpAttack UMETA(DisplayName = "Jump Attack")
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHandledTrace, class ACharacter*, InAttacker,class AActor*, InAttackCauser, class ACharacter*, InOther);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHandledJumpTrace, class ACharacter*, InAttacker, class AActor*, InAttackCauser, class ACharacter*, InOther);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHandledParryTrace, class ACharacter*, InAttacker, class AActor*, InAttackCauser, class ACharacter*, InOther);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndTrace);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class P2406_JS_API UCAttackTraceComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	  // Ʈ���̽� ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Trace")
    float TraceRadius = 50.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Trace")
    FName StartSocketName = "TraceStart";

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Trace")
    FName EndSocketName = "TraceEnd";

public:	
	UCAttackTraceComponent();

protected:
	void BeginPlay() override;
    void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
    void SetBeginTrace();     
    UFUNCTION()
    void SetEndTrace();
  
    void HandleTrace(AActor* InHitActor);

private: 
    bool HandleAriborneTrace();

public:
	UFUNCTION()
    void OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType);
    UFUNCTION()
    void OnNormalAttack();
    UFUNCTION()
    void OnParryAttack();
    UFUNCTION()
    void OnJumpAttack();

private:
    bool GetMyTeam(class AActor* InHitTarget);

public:
    FOnHandledTrace OnHandledTrace;
    FOnHandledParryTrace OnHandledParryTrace; 
    FOnHandledJumpTrace OnHandledJumpTrace;
    FOnEndTrace OnEndTrace; 

private:
    bool bIsAttacking = false; 

    // ĸ�� ũ�� ���� (���� ���� ���� ����)
    float CapsuleRadius = 50.0f;  // ���� ����
    float CapsuleHalfHeight = 100.0f; // ���� ���� Ȯ��
    
    EAttackType CurrentType; 

private:
    TArray<class ACharacter*> Hits; 
    class ACharacter* OwnerCharacter; 
    class UCWeaponComponent* Weapon; 
    class ACAttachment* Attachment;
};
