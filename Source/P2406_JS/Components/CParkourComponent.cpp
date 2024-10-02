#include "Components/CParkourComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CMovementComponent.h"


void FParkourData::PlayMontage(ACharacter* InCharacter)
{
	if (bFixedCamera)
	{
		UCMovementComponent* movement = CHelpers::GetComponent<UCMovementComponent>(InCharacter);

		if (!!movement)
			movement->EnableFixedCamera();
	}

	InCharacter->PlayAnimMontage(Montage, PlayRate, SectionName);
}

//-----------------------------------------------------------------------------------

UCParkourComponent::UCParkourComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	CHelpers::GetAsset<UDataTable>(&DataTable, "DataTable'/Game/Parkour/DT_Parkour.DT_Parkour'");
}


// Called when the game starts
void UCParkourComponent::BeginPlay()
{
	Super::BeginPlay();

	TArray<FParkourData*> rows;
	DataTable->GetAllRows<FParkourData>("", rows);

	for (int32 i = 0; i < (int32)EParkourType::Max; i++)
	{
		TArray<FParkourData> temp;
		for (FParkourData* data : rows)
		{
			if (data->Type == (EParkourType)i)
				temp.Add(*data);
		}

		DataMap.Add((EParkourType)i, temp);
	}

#ifdef LOG_UCParkourComponent
	for (TPair<EParkourType, TArray<FParkourData>> temp : DataMap)
	{
		CLog::Log(StaticEnum<EParkourType>()->GetNameStringByValue((uint8)temp.Key));

		for (FParkourData tempData : temp.Value)
		{
			CLog::Log(tempData.Montage->GetPathName());
		}
	}
#endif // LOG_UCParkourComponent



	OwnerCharacter = Cast<ACharacter>(GetOwner());
	CheckNull(OwnerCharacter);


	USceneComponent* arrows = CHelpers::GetComponent<USceneComponent>(OwnerCharacter, "Arrows");

	TArray<USceneComponent*> components;
	arrows->GetChildrenComponents(false, components);

	for (int32 i = 0; i < (int32)EParkourArrowType::Max; i++)
		Arrows[i] = Cast<UArrowComponent>(components[i]);
	
}


// Called every frame
void UCParkourComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckTrace_Land();
}


void UCParkourComponent::DoParkour(bool bLanded)
{
	CheckFalse(Type == EParkourType::Max);

	if (bLanded && Check_FallMode())
	{
		DoParkour_Fall();

		return;
	}


	HitObstacle = nullptr;
	HitObstacleExtent = FVector::ZeroVector;
	HitDistance = 0.0f;
	ToFrontYaw = 0.0f;

	CheckTrace_Center();

	if (!!HitObstacle)
	{
		CheckTrace_Head();
		CheckTrace_Foot();
		CheckTrace_Side();
	}


	CheckFalse(Check_Obstacle());

	if (Check_ClimbMode())
	{
		DoParkour_Climb();

		return;
	}

	if (Check_SlideMode())
	{
		DoParkour_Slide();

		return;
	}


	FParkourData data;
	if (Check_ObstacleMode(EParkourType::Normal, data))
	{
		DoParkour_Obstacle(EParkourType::Normal, data);

		return;
	}

	if (Check_ObstacleMode(EParkourType::Short, data))
	{
		DoParkour_Obstacle(EParkourType::Short, data);

		return;
	}

	if (Check_ObstacleMode(EParkourType::Wall, data))
	{
		DoParkour_Obstacle(EParkourType::Wall, data);

		return;
	}
}

void UCParkourComponent::End_DoParkour()
{
	switch (Type)
	{
	case EParkourType::Climb:
		End_DoParkour_Climb();
		break;

	case EParkourType::Fall:
		End_DoParkour_Fall();
		break;

	case EParkourType::Slide:
		End_DoParkour_Slide();
		break;

	case EParkourType::Short:
	case EParkourType::Normal:
	case EParkourType::Wall:
		End_DoParkour_Obstacle();
		break;
	}

	Type = EParkourType::Max;

	UCMovementComponent* movement = CHelpers::GetComponent<UCMovementComponent>(OwnerCharacter);

	if (!!movement)
	{
		if (movement->GetFixedCamera() == true)
			movement->DisableFixedCamera();
	}
}

void UCParkourComponent::LineTrace(EParkourArrowType InType)
{
	UArrowComponent* arrow = Arrows[(int32)InType];
	FLinearColor color = FLinearColor(arrow->ArrowColor);

	FTransform transform = arrow->GetComponentToWorld();

	FVector start = transform.GetLocation();
	FVector end = start + OwnerCharacter->GetActorForwardVector() * TraceDistance;


	TArray<AActor*> ignores;
	ignores.Add(OwnerCharacter);

	UKismetSystemLibrary::LineTraceSingle(GetWorld(), start, end, ETraceTypeQuery::TraceTypeQuery3, false, ignores, DebugType, HitResults[(int32)InType], true, color, FLinearColor::White);
}

void UCParkourComponent::CheckTrace_Center()
{
	EParkourArrowType type = EParkourArrowType::Center;
	LineTrace(type);

	const FHitResult& hitResult = HitResults[(int32)type];
	CheckFalse(hitResult.bBlockingHit);


	UStaticMeshComponent* mesh = CHelpers::GetComponent<UStaticMeshComponent>(hitResult.GetActor());
	CheckNull(mesh);


	HitObstacle = hitResult.GetActor();

	FVector minBound, maxBound;
	mesh->GetLocalBounds(minBound, maxBound);

	float x = FMath::Abs(minBound.X - maxBound.X);
	float y = FMath::Abs(minBound.Y - maxBound.Y);
	float z = FMath::Abs(minBound.Z - maxBound.Z);
	HitObstacleExtent = FVector(x, y, z);

	HitDistance = hitResult.Distance;

	ToFrontYaw = UKismetMathLibrary::MakeRotFromX(-hitResult.ImpactNormal).Yaw;


#ifdef LOG_UCParkourComponent
	CLog::Print("HitObstacle : " + HitObstacle->GetName(), 1100);
	CLog::Print("HitObstacleExtent : " + HitObstacleExtent.ToString(), 1101);
	CLog::Print("HitDistance : " + FString::SanitizeFloat(HitDistance), 1102);
	CLog::Print("ToFrontYaw : " + FString::SanitizeFloat(ToFrontYaw), 1103);
#endif
}

void UCParkourComponent::CheckTrace_Head()
{
	LineTrace(EParkourArrowType::Head);
}

void UCParkourComponent::CheckTrace_Foot()
{
	LineTrace(EParkourArrowType::Foot);
}

void UCParkourComponent::CheckTrace_Side()
{
	LineTrace(EParkourArrowType::Left);
	LineTrace(EParkourArrowType::Right);
}

bool UCParkourComponent::Check_Obstacle()
{
	CheckNullResult(HitObstacle, false);


	bool b = true;
	b &= HitResults[(int32)EParkourArrowType::Center].bBlockingHit;
	b &= HitResults[(int32)EParkourArrowType::Left].bBlockingHit;
	b &= HitResults[(int32)EParkourArrowType::Right].bBlockingHit;
	CheckFalseResult(b, false);


	FVector center = HitResults[(int32)EParkourArrowType::Center].ImpactNormal;
	FVector left = HitResults[(int32)EParkourArrowType::Left].ImpactNormal;
	FVector right = HitResults[(int32)EParkourArrowType::Right].ImpactNormal;

	CheckFalseResult(center.Equals(left), false);
	CheckFalseResult(center.Equals(right), false);


	FVector start = HitResults[(int32)EParkourArrowType::Center].ImpactPoint;
	FVector end = OwnerCharacter->GetActorLocation();
	float lookAt = UKismetMathLibrary::FindLookAtRotation(start, end).Yaw;

	FVector impactNormal = HitResults[(int32)EParkourArrowType::Center].ImpactNormal;
	float impactAt = UKismetMathLibrary::MakeRotFromX(impactNormal).Yaw;

	float yaw = FMath::Abs(FMath::Abs(lookAt) - FMath::Abs(impactAt));
	CheckFalseResult(yaw <= AvailableFrontAngle, false);


#ifdef LOG_UCParkourComponent
	CLog::Print("lookAt : " + FString::SanitizeFloat(lookAt), 1110);
	CLog::Print("impactNormal : " + impactNormal.ToString(), 1111);
	CLog::Print("impactAt : " + FString::SanitizeFloat(impactAt), 1112);
#endif

	return true;
}

bool UCParkourComponent::Check_ClimbMode()
{
	CheckFalseResult(HitResults[(int32)EParkourArrowType::Head].bBlockingHit, false);

	const TArray<FParkourData>* datas = DataMap.Find(EParkourType::Climb);
	CheckFalseResult((*datas)[0].MinDistance < HitDistance, false);
	CheckFalseResult((*datas)[0].MaxDistance > HitDistance, false);
	CheckFalseResult(FMath::IsNearlyEqual((*datas)[0].Extent, HitObstacleExtent.Z, 10), false);

	return true;
}

void UCParkourComponent::DoParkour_Climb()
{
	Type = EParkourType::Climb;

	OwnerCharacter->SetActorLocation(HitResults[(int32)EParkourArrowType::Center].ImpactPoint);
	OwnerCharacter->SetActorRotation(FRotator(0, ToFrontYaw, 0));
	(*DataMap.Find(EParkourType::Climb))[0].PlayMontage(OwnerCharacter);

	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
}

void UCParkourComponent::End_DoParkour_Climb()
{
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void UCParkourComponent::CheckTrace_Land()
{
	CheckFalse(OwnerCharacter->GetCharacterMovement()->IsFalling());
	CheckTrue(bFirstFalling);

	bFirstFalling = true;


	UArrowComponent* arrow = Arrows[(int32)EParkourArrowType::Land];
	FLinearColor color = FLinearColor(arrow->ArrowColor);

	FTransform transform = arrow->GetComponentToWorld();
	FVector start = transform.GetLocation();

	const TArray<FParkourData>* datas = DataMap.Find(EParkourType::Fall);
	FVector end = start + transform.GetRotation().GetForwardVector() * (*datas)[0].Extent;


	TArray<AActor*> ignores;
	ignores.Add(OwnerCharacter);

	UKismetSystemLibrary::LineTraceSingle(GetWorld(), start, end, ETraceTypeQuery::TraceTypeQuery1, false, ignores, DebugType, HitResults[(int32)EParkourArrowType::Land], true, color, FLinearColor::White);
}

bool UCParkourComponent::Check_FallMode()
{
	CheckFalseResult(bFirstFalling, false);
	bFirstFalling = false;


	float distance = HitResults[(int32)EParkourArrowType::Land].Distance;

	const TArray<FParkourData>* datas = DataMap.Find(EParkourType::Fall);
	CheckFalseResult((*datas)[0].MinDistance < distance, false);
	CheckFalseResult((*datas)[0].MaxDistance > distance, false);

	return true;
}

void UCParkourComponent::DoParkour_Fall()
{
	Type = EParkourType::Fall;

	(*DataMap.Find(EParkourType::Fall))[0].PlayMontage(OwnerCharacter);
}

void UCParkourComponent::End_DoParkour_Fall()
{

}

bool UCParkourComponent::Check_SlideMode()
{
	CheckTrueResult(HitResults[(int32)EParkourArrowType::Foot].bBlockingHit, false);

	const TArray<FParkourData>* datas = DataMap.Find(EParkourType::Slide);
	CheckFalseResult((*datas)[0].MinDistance < HitDistance, false);
	CheckFalseResult((*datas)[0].MaxDistance > HitDistance, false);


	UArrowComponent* arrow = Arrows[(int32)EParkourArrowType::Foot];
	FLinearColor color = FLinearColor(arrow->ArrowColor);

	FTransform transform = arrow->GetComponentToWorld();

	FVector arrowLocation = transform.GetLocation();
	FVector ownerLocation = OwnerCharacter->GetActorLocation();


	float extent = (*datas)[0].Extent; //30
	float z = arrowLocation.Z + extent;

	FVector forward = OwnerCharacter->GetActorForwardVector();
	forward = FVector(forward.X, forward.Y, 0);

	FVector start = FVector(arrowLocation.X, arrowLocation.Y, z);
	FVector end = start + forward * TraceDistance;


	TArray<AActor*> ignores;
	FHitResult hitResult;

	UKismetSystemLibrary::BoxTraceSingle(GetWorld(), start, end, FVector(0, extent, extent), OwnerCharacter->GetActorRotation(), ETraceTypeQuery::TraceTypeQuery1, false, ignores, DebugType, hitResult, true, color, FLinearColor::White);
	CheckTrueResult(hitResult.bBlockingHit, false);


	return true;
}

void UCParkourComponent::DoParkour_Slide()
{
	Type = EParkourType::Slide;

	OwnerCharacter->SetActorRotation(FRotator(0, ToFrontYaw, 0));
	(*DataMap.Find(EParkourType::Slide))[0].PlayMontage(OwnerCharacter);

	BackupObstacle = HitObstacle;
	BackupObstacle->SetActorEnableCollision(false);
}

void UCParkourComponent::End_DoParkour_Slide()
{
	BackupObstacle->SetActorEnableCollision(true);
	BackupObstacle = nullptr;
}

bool UCParkourComponent::Check_ObstacleMode(EParkourType InType, FParkourData& OutData)
{
	CheckTrueResult(HitResults[(int32)EParkourArrowType::Head].bBlockingHit, false);

	const TArray<FParkourData>* datas = DataMap.Find(InType);

	for (int32 i = 0; i < (*datas).Num(); i++)
	{
		bool b = true;
		b &= (*datas)[i].MinDistance < HitDistance;
		b &= (*datas)[i].MaxDistance > HitDistance;
		b &= FMath::IsNearlyEqual((*datas)[i].Extent, HitObstacleExtent.Y, 10);

		OutData = (*datas)[i];
		CheckTrueResult(b, true);
	}

	return false;
}

void UCParkourComponent::DoParkour_Obstacle(EParkourType InType, FParkourData& InData)
{
	Type = InType;

	OwnerCharacter->SetActorRotation(FRotator(0, ToFrontYaw, 0));
	InData.PlayMontage(OwnerCharacter);

	BackupObstacle = HitObstacle;
	BackupObstacle->SetActorEnableCollision(false);
}

void UCParkourComponent::End_DoParkour_Obstacle()
{
	BackupObstacle->SetActorEnableCollision(true);
	BackupObstacle = nullptr;
}