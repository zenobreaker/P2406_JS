#include "Weapons/SubActions/CSubAction_Sword_Execute.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"

#include "Characters/CBaseCharacter.h"

void UCSubAction_Sword_Execute::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCDoAction* InDoAction)
{
	Super::BeginPlay(InOwner, InAttachment, InDoAction);

}

void UCSubAction_Sword_Execute::Pressed()
{
	Super::Pressed();
	
	CheckTrue(bExecuting);

	//bExecuting = true;
	DoExecute();
}

class ACharacter* UCSubAction_Sword_Execute::CheckFrontNearTarget()
{
	CheckNullResult(Owner, nullptr); 

	float radius = Owner->GetCapsuleComponent()->GetScaledCapsuleRadius();

	FVector forward = Owner->GetActorForwardVector();
	FVector start = Owner->GetActorLocation();
	FVector end = start + forward * Distance;

	TArray<AActor*> ignores;
	ignores.Add(Owner);
	FHitResult hitResult; 

	// ĳ������ ĸ�� ������Ʈ ũ�� ��ŭ ���濡 Ʈ���̽� �߻� 
	UKismetSystemLibrary::SphereTraceSingle(Owner->GetWorld(), start, end,
		radius, ETraceTypeQuery::TraceTypeQuery8, false, ignores, EDrawDebugTrace::None,
		hitResult, true, FLinearColor::White);

	return Cast<ACharacter>(hitResult.GetActor());
}

void UCSubAction_Sword_Execute::DoExecute()
{
	CheckNull(Owner);
	CheckFalse(ActionDatas.Num() > 0);
	CheckFalse(HitDatas.Num() > 0);
	
	Target = CheckFrontNearTarget();
	CheckNull(Target);

	ACBaseCharacter* base = Cast<ACBaseCharacter>(Target);
	CheckNull(base);
	CheckFalse(base->GetExecutable());

	FVector start = Target->GetActorLocation();
	FVector target = Owner->GetActorLocation();
	FVector direction = target - start;
	direction.Normalize();

	//bExecuting = true;

	// 1. Ÿ���� ������ ���� 
	FRotator targetRotator = UKismetMathLibrary::FindLookAtRotation(start, target);
	targetRotator.Pitch = 0;
	Target->SetActorRotation(targetRotator);

	// 2. ��� ���� 
	ActionDatas[0].DoAction(Owner);

	//TODO : �� ������ ������ ���� ���� ����������
	// 3. �� ��ǵ� ���� 
	Target->PlayAnimMontage(HitDatas[0].Montage, HitDatas[0].PlayRate);
	
	//4. Target���� ó�� �÷��� ������ 
	//
}
