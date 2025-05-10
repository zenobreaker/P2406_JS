#include "Components/CTargetComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Particles/ParticleSystemComponent.h"

#include "Characters/CBaseCharacter.h"

#include "Components/CStateComponent.h"
#include "Components/WidgetComponent.h"

UCTargetComponent::UCTargetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	//FHelpers::GetAsset<UParticleSystem>(&ParticleAsset, "/Script/Engine.ParticleSystem'/Game/AdvancedMagicFX12/particles/P_ky_aura_shine.P_ky_aura_shine'");

	FHelpers::GetClass<UUserWidget>(&TargetUiClass, "/Script/UMGEditor.WidgetBlueprint'/Game/Widgets/TargetCrossHair.TargetCrossHair_C'");
}


void UCTargetComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
}


void UCTargetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	CheckNull(Target);

	UCStateComponent* state = FHelpers::GetComponent<UCStateComponent>(Target);
	CheckNull(state);
	CheckTrue(state->IsDeadMode());

	float distance = OwnerCharacter->GetDistanceTo(Target);
	if (distance > TraceDistance)
	{
		End();

		return; 
	}

	FRotator controlRotation = OwnerCharacter->GetControlRotation();
	FRotator ownerToTarget = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), Target->GetActorLocation());
	ownerToTarget.Pitch = controlRotation.Pitch;

	APlayerController* controller = OwnerCharacter->GetController<APlayerController>(); 

	if (controlRotation.Equals(ownerToTarget, FinishAngle))
	{
		bMovingFocus = false; 
		controller->SetControlRotation(ownerToTarget);

		return; 
	}

	FRotator targetRotation = FRotator(controlRotation.Pitch, ownerToTarget.Yaw, ownerToTarget.Roll);
	FRotator result = UKismetMathLibrary::RInterpTo(controlRotation, targetRotation, DeltaTime, InterpSpeed);

	controller->SetControlRotation(result); 
}

void UCTargetComponent::Toggle()
{
	if (Target == nullptr)
	{
		Start();

		return;
	}

	End();
}

void UCTargetComponent::Start()
{
	FVector location = OwnerCharacter->GetActorLocation();


	TArray<AActor*> ignores;
	ignores.Add(OwnerCharacter);

	TArray<FHitResult> hitResults;
	UKismetSystemLibrary::SphereTraceMultiByProfile(GetWorld(), location, location, TraceDistance, "Targeting", false, ignores, DrawDebug, hitResults, true);

	ACharacter* candidate = GetNearlyFrontAngle(hitResults);
	Change(candidate);
}

void UCTargetComponent::End()
{
	if (!!TargetUi)
		TargetUi->DestroyComponent();

	Target = nullptr;
}

void UCTargetComponent::Change(ACharacter* InCandidate)
{
	if (InCandidate == nullptr)
	{
		End();

		return;
	}


	if (!!TargetUi)
		TargetUi->DestroyComponent();

	if (TargetUiClass != nullptr)
	{
		TargetUi = NewObject<UWidgetComponent>(InCandidate);
		if (!!TargetUi)
		{
			TargetUi->AttachToComponent(InCandidate->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
			TargetUi->RegisterComponent();
			FTransform transform;
			transform.SetLocation(FVector(0, 0, 50));
			TargetUi->SetRelativeTransform(transform);

			TargetUi->SetWidgetClass(TargetUiClass);
			TargetUi->SetWidgetSpace(EWidgetSpace::Screen);
			TargetUi->SetDrawSize(FVector2D(100, 100)); // 적절한 크기 설정
			TargetUi->SetVisibility(true);
		}
	}

	Target = InCandidate;
}

AActor* UCTargetComponent::GetNearlyFrontAngleActor(const TArray<FHitResult>& InHitResult)
{
	float angle = -2.0f; 
	AActor* candidate = nullptr; 

	for (int i = 0; i < InHitResult.Num(); i++)
	{
		FVector targetLocation = InHitResult[i].GetActor()->GetActorLocation(); 
		FVector direction = targetLocation - OwnerCharacter->GetActorLocation(); 
		direction.Normalize(); 

		FRotator rotator = OwnerCharacter->GetControlRotation(); 
		FVector forward = FQuat(rotator).GetForwardVector(); 
		float dot = FVector::DotProduct(direction, forward); 

		if (dot >= angle)
		{
			angle = dot; 
			candidate = InHitResult[i].GetActor(); 
		}
	}

	return candidate;
}

ACharacter* UCTargetComponent::GetNearlyFrontAngle(const TArray<FHitResult>& InHitResults)
{
	float angle = -2.0f;
	ACharacter* candidate = nullptr;

	for (int i = 0; i < InHitResults.Num(); i++)
	{
		ACBaseCharacter* bc = Cast<ACBaseCharacter>(InHitResults[i].GetActor());
		ACBaseCharacter* oc = Cast<ACBaseCharacter>(OwnerCharacter);
		if (!!bc)
		{
			if (FGenericTeamId::GetAttitude(bc->GetGenericTeamId(), oc->GetGenericTeamId())
				== ETeamAttitude::Type::Friendly)
			{
				continue;
			}
		}

		FVector targetLocation = InHitResults[i].GetActor()->GetActorLocation();

		FVector direction = targetLocation - OwnerCharacter->GetActorLocation();
		direction.Normalize();


		FRotator rotator = OwnerCharacter->GetControlRotation();
		FVector forward = FQuat(rotator).GetForwardVector();

		float dot = FVector::DotProduct(direction, forward);

		//FString str;
		//str.Append(InHitResults[i].GetActor()->GetActorLabel());
		//str.Append(" : ");
		//str.Append(FString::SanitizeFloat(dot));

		//CLog::Log(str);


		if (dot >= angle)
		{
			angle = dot;
			candidate = Cast<ACharacter>(InHitResults[i].GetActor());
		}
	}

	return candidate;
}



void UCTargetComponent::MoveLeft()
{
	Move(false);
}

void UCTargetComponent::MoveRight()
{
	Move(true);
}

void UCTargetComponent::Move(bool bRight)
{
	CheckTrue(bMovingFocus);


	FVector location = OwnerCharacter->GetActorLocation();

	TArray<AActor*> ignores;
	ignores.Add(OwnerCharacter);
	ignores.Add(Target);

	TArray<FHitResult> hitResults;
	UKismetSystemLibrary::SphereTraceMultiByProfile(GetWorld(), location, location, TraceDistance, "Targeting", false, ignores, DrawDebug, hitResults, true);


	TMap<float, ACharacter*> map;

	for (const FHitResult& hitResult : hitResults)
	{
		ACharacter* character = Cast<ACharacter>(hitResult.GetActor());
		if (character == nullptr)
			continue;


		FVector forward = FQuat(OwnerCharacter->GetControlRotation()).GetForwardVector();


		FVector location2 = character->GetActorLocation();

		FVector direction = location2 - location;
		direction.Normalize();


		FVector cross = FVector::CrossProduct(forward, direction);
		float dot = FVector::DotProduct(FVector::UpVector, cross);

		map.Add(dot, character);
	}


	float minimum = FLT_MAX;
	ACharacter* candidate = nullptr;

	for (TPair<float, ACharacter*> pair : map)
	{
		float key = pair.Key;

		if (bRight)
		{
			if (key < 0.0f)
				continue;
		}
		else
		{
			if (key > 0.0f)
				continue;
		}


		if (FMath::Abs(key) < minimum)
		{
			minimum = FMath::Abs(key);
			candidate = pair.Value;
		}
	}

	bMovingFocus = true;
	Change(candidate);
}




