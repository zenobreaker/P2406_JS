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
	
	// 메시 갖고옴 
	USkeletalMeshComponent* mesh = FHelpers::GetComponent<USkeletalMeshComponent>(Weapon->GetAttachment());
	CheckNull(mesh);
	
	// 위치 갱신 
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
	// 간단한 충돌 처리
	// 
	 // 캐릭터 여부 확인
	ACharacter* hitCharacter = Cast<ACharacter>(InHitActor);
	CheckNull(hitCharacter);

	// 이미 무기로 충돌 처리된 경우 무시
	if (hitCharacter->Tags.Contains(FName("HitByWeapon")))
		return;

	// 트레이스 처리
	Hits.AddUnique(hitCharacter);
	UE_LOG(LogTemp, Log, TEXT("Trace Hit Actor: %s"), *InHitActor->GetName());

	CheckNull(Weapon->GetAttachment());
	if (OnHandledTrace.IsBound())
	{
		OnHandledTrace.Broadcast(OwnerCharacter, Weapon->GetAttachment(), hitCharacter);
	}
	
	// 필요한 추가 처리
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
//	// SphereTrace 실행
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
//	// 디버그 확인
//	DrawDebugSphere(GetWorld(), Start, TraceRadius, 12, FColor::Green, false, 1.0f);
//	DrawDebugSphere(GetWorld(), End, TraceRadius, 12, FColor::Red, false, 1.0f);
//}

