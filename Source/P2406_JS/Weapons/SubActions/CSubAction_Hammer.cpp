#include "Weapons/SubActions/CSubAction_Hammer.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/CAttachment.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"


UCSubAction_Hammer::UCSubAction_Hammer()
{
	//TODO: SubAction�� Reset �Լ��� ���� ���⿡ Reset ��� �־�α�?
}

void UCSubAction_Hammer::Pressed()
{
	CheckFalse(ActionDatas.Num() > 0);
	CheckTrue(State->IsActionMode());

	CLog::Print("Hammer SubAction Start");


	ResetCharging();

	IsCharging = true;
	//ActionDatas[0].DoAction(Owner);
	Owner->PlayAnimMontage(ChargeAnimMontage, 0.0f);

	Owner->GetWorld()->GetTimerManager().SetTimer(ChargeimeHandle, this, &UCSubAction_Hammer::CreateEffect_Charging, 0.75f, false);
	State->SetActionMode();
}

void UCSubAction_Hammer::Released()
{

	// ���� ���°� �Ϸ� �Ǽ� ���� �����̸� 
	if (bActionable == true)
	{
		// ���� �׼� ����
		ActionDatas[0].DoAction(Owner);

		State->SetIdleMode();
		bActionable = false;
		return;
	}
	else
		Owner->PlayAnimMontage(ChargeAnimMontage, 1.0f);

	ResetCharging();
}

void UCSubAction_Hammer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckFalse(IsCharging == true);

	CurrentChargeTime += DeltaTime; // ���� �ð� ����
	CurrentChargeTime = FMath::Min(CurrentChargeTime, MaxChargeTime);

	if (CurrentChargeTime >= MaxChargeTime)
	{
		// ���� �Ϸ� �ִ� ���
		CLog::Print("Sub Action Charge Complete", 1, 2);
		bActionable = true;
	}
}

void UCSubAction_Hammer::CreateEffect_Charging()
{
	CheckFalse(Effects.Num() > 0);
	CheckNull(Owner);

	Effects[0].PlaySoundWave(Owner);


	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(Owner);
	CheckNull(weapon);

	class ACAttachment* hammer =  weapon->GetAttachment();
	CheckNull(hammer);

	USkeletalMeshComponent * skeletalMesh = CHelpers::GetComponent<USkeletalMeshComponent>(hammer, "SkeletalMesh");
	CheckNull(skeletalMesh);

	if (skeletalMesh->DoesSocketExist("Hammer_Head"))
	{
		FVector socketLocation = skeletalMesh->GetSocketLocation("Hammer_Head");
		CLog::Log("Hammer_Head Socket Location:" + socketLocation.ToString());
	}
	else if(skeletalMesh == nullptr){
		CLog::Log("skeletalMesh is null!");
	}
	else if (skeletalMesh->DoesSocketExist("Hammer_Head")) {
		CLog::Log("Soket Not foind !");
	}


	NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
		Effects[0].Effect,
		skeletalMesh,
		"Hammer_Heed",  // ���� �̸�
		FVector::ZeroVector,  // ������ �⺻ ��ġ�� ȸ�� ���
		FRotator::ZeroRotator,
		EAttachLocation::SnapToTarget,
		true  // ����Ʈ�� ���⸦ ����ٴϵ��� ����
	);

}

void UCSubAction_Hammer::CreateEffectSubActionHammer()
{
	CheckFalse(Effects.Num() > 0);
	CheckNull(Owner);

	if(NiagaraComponent != nullptr)
		NiagaraComponent->DestroyComponent();

	Effects[1].PlaySoundWave(Owner);

	FVector forward = Owner->GetActorForwardVector() * 500.0f;
	FVector locatoion = Owner->GetActorLocation(); 
	Effects[1].PlayEffect(Owner, forward + locatoion, Owner->GetActorRotation());
}


// ������ �ʱ�ȭ
void UCSubAction_Hammer::ResetCharging()
{
	IsCharging = false;
	bActionable = false;
	CurrentChargeTime = 0.0f;

	DamagedList.Empty();

	Owner->GetWorld()->GetTimerManager().ClearTimer(ChargeimeHandle);
	NiagaraComponent = nullptr;
	
}

void UCSubAction_Hammer::TraceAttackArea()
{
	// ���Ǿ� Ʈ���̽��� �ֺ� ���� ���� 
	TArray<FHitResult> HitResults;
	FVector StartLocation = Owner->GetActorLocation();
	FVector forward = Owner->GetActorForwardVector() * 500.0f;
	StartLocation += forward;
	FVector EndLocation = StartLocation;  // ���Ǿ�� ���۰� ���� �����ص� ��

	float SphereRadius = 250;  // ���� �ݰ�
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(Owner);

	bool bHit = UKismetSystemLibrary::SphereTraceMultiByProfile(Owner->GetWorld(),
		StartLocation, EndLocation, SphereRadius, "Targeting",
		false, IgnoredActors, EDrawDebugTrace::ForDuration, HitResults,
		true, FLinearColor::Green, FLinearColor::Red, 1.0f);

	if (bHit && HitResults.Num() > 0)
	{
		for (const FHitResult& hitResult : HitResults)
		{
			ACharacter* enemy = Cast<ACharacter>(hitResult.GetActor());
			if (enemy == NULL || DamagedList.Contains(enemy))
				continue;

			DamagedList.Add(enemy);

			HitDatas[0].SendDamage(Owner, Attachment, enemy);

		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
void FEffectData::PlaySoundWave(ACharacter* InOwner)
{
	CheckNull(Sound);

	UWorld* world = InOwner->GetWorld();
	FVector location = InOwner->GetActorLocation();

	UGameplayStatics::SpawnSoundAtLocation(world, Sound, location);
}

void FEffectData::PlayEffect(class ACharacter* InOwner, const FVector& InLocation)
{
	CheckNull(Effect);


	FRotator spawnRotator = InOwner->GetActorRotation();
	//spawnRotator.Roll = angle;

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		InOwner->GetWorld(),
		Effect,
		InLocation,
		spawnRotator,
		EffectScale
	);
}

void FEffectData::PlayEffect(class ACharacter* InOwner, const FVector& InLocation, const FRotator& InRotator)
{
	CheckNull(Effect);

	FTransform transform;

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		InOwner->GetWorld(),
		Effect,
		InLocation,
		InRotator,
		EffectScale
	);
}
