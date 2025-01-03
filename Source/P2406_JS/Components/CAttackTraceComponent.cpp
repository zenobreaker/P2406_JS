#include "Components/CAttackTraceComponent.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"

#include "Weapons/CAttachment.h"

UCAttackTraceComponent::UCAttackTraceComponent()
{

	PrimaryComponentTick.bCanEverTick = true;
	
}


void UCAttackTraceComponent::BeginPlay()
{
	Super::BeginPlay();


	OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!!OwnerCharacter)
		Weapon = FHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter);

	REGISTER_EVENT_WITH_REPLACE(Weapon, OnWeaponTypeChanged, this, UCAttackTraceComponent::OnWeaponTypeChanged);

}

void UCAttackTraceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckFalse(bIsAttacking);
	CheckNull(OwnerCharacter);
	CheckNull(Weapon);
	CheckNull(Weapon->GetAttachment());

	ACAttachment* attachment = Weapon->GetAttachment();
	CheckNull(attachment);

	FHitResult TraceHitResult;
	FCollisionQueryParams QueryParams;

	QueryParams.AddIgnoredActor(OwnerCharacter); 
	QueryParams.AddIgnoredActor(attachment);
	
	// �޽� ����� 
	USkeletalMeshComponent* mesh = FHelpers::GetComponent<USkeletalMeshComponent>(Weapon->GetAttachment());
	CheckNull(mesh);
	
	// ��ġ ���� 
	FVector newVec = mesh->GetSocketLocation(attachment->GetTraceGoalName());
	EndVec = newVec;
	StartVec = OwnerCharacter->GetActorLocation();

	bool bCheck = GetWorld()->LineTraceSingleByChannel(TraceHitResult, StartVec, EndVec, ECC_Pawn, QueryParams);
	
	if(bCheck)
	{
		for (ACharacter* hit : Hits)
			CheckTrue(hit == TraceHitResult.GetActor());

		Hits.AddUnique(Cast<ACharacter>(TraceHitResult.GetActor()));
		HandleTrace(TraceHitResult.GetActor());

	
	}
	FColor LineColor = bCheck ? FColor::Red : FColor::Blue;
	DrawDebugLine(GetWorld(), StartVec, EndVec, LineColor, false, 1);
}

void UCAttackTraceComponent::SetTrace()
{
	//StartVec = InStart;
	//EndVec = InEnd; 

	bIsAttacking = true; 
}

void UCAttackTraceComponent::SetEndTrace()
{
	 bIsAttacking = false; 

	 Hits.Empty();

	 DYNAMIC_EVENT_CALL(OnEndTrace); 
}

void UCAttackTraceComponent::HandleTrace(AActor* InHitActor)
{
	CheckNull(InHitActor);
	// ������ �浹 ó��
	// 
	 // ĳ���� ���� Ȯ��
	ACharacter* hitCharacter = Cast<ACharacter>(InHitActor);
	CheckNull(hitCharacter);

	// �̹� ����� �浹 ó���� ��� ����
	if (hitCharacter->Tags.Contains(FName("HitByWeapon")))
		return;

	// Ʈ���̽� ó��
	Hits.AddUnique(hitCharacter);
	UE_LOG(LogTemp, Log, TEXT("Trace Hit Actor: %s"), *InHitActor->GetName());

	CheckNull(Weapon->GetAttachment());
	if (OnHandledTrace.IsBound())
	{
		OnHandledTrace.Broadcast(OwnerCharacter, Weapon->GetAttachment(), hitCharacter);
	}
	
	// �ʿ��� �߰� ó��
	Weapon->GetAttachment()->HandleAttachmentOverlap(OwnerCharacter, Weapon->GetAttachment(), hitCharacter);
}

void UCAttackTraceComponent::OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType)
{
	SetEndTrace();
}



//
//void UCAttackTraceComponent::PerformTrace(AActor* OwningWeapon, TArray<FHitResult>& OutHits)
//{
//	CheckNull(OwningWeapon); 
//
//	USkeletalMeshComponent* skeletal = FHelpers::GetComponent<USkeletalMeshComponent>(OwningWeapon);
//	CheckNull(skeletal);
//
//	FVector Start = skeletal->GetSocketLocation(StartSocketName);
//	FVector End = skeletal->GetSocketLocation(EndSocketName);
//
//
//	FCollisionQueryParams Params;
//	Params.AddIgnoredActor(OwningWeapon);
//	Params.AddIgnoredActor(GetOwner());
//
//	// SphereTrace ����
//	GetWorld()->SweepMultiByChannel(
//		OutHits,
//		Start,
//		End,
//		FQuat::Identity,
//		ECC_Pawn,
//		FCollisionShape::MakeSphere(TraceRadius),
//		Params
//	);
//
//	// ����� Ȯ��
//	DrawDebugSphere(GetWorld(), Start, TraceRadius, 12, FColor::Green, false, 1.0f);
//	DrawDebugSphere(GetWorld(), End, TraceRadius, 12, FColor::Red, false, 1.0f);
//}

