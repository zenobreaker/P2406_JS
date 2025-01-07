#include "BehaviorTree/EQT/CEnvQueryTest_ExcludeCurLocation.h"
#include "Global.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_VectorBase.h"
#include "GameFramework/Character.h"

UCEnvQueryTest_ExcludeCurLocation::UCEnvQueryTest_ExcludeCurLocation()
{
	Cost = EEnvTestCost::Low;
	ValidItemType = UEnvQueryItemType_VectorBase::StaticClass(); // 쿼리 아이템 타입 설정
}

void UCEnvQueryTest_ExcludeCurLocation::RunTest(FEnvQueryInstance& QueryInstance) const
{
	bool bWantNear = BoolValue.GetValue();

	// 제외할 아이템의 인덱스를 저장할 변수
	int32 ExcludedItemIndex = -1;
	float ClosestDistance = FLT_MAX; // 가장 가까운 아이템의 거리

	//// 현재 쿼리에서 가장 적합한 아이템을 찾는 로직 (예: 가까운 거리의 아이템)
	//for (int32 Index = 0; Index < QueryInstance.Items.Num(); ++Index)
	//{
	//	// 아이템의 위치를 가져옴
	//	FVector ItemLocation = GetItemLocation(QueryInstance, Index);
	//	float Distance = FVector::Dist(ItemLocation, TargetLocation);

	//	// 가장 가까운 아이템을 제외할 인덱스로 지정
	//	if (Distance < ClosestDistance)
	//	{
	//		ClosestDistance = Distance;
	//		ExcludedItemIndex = Index;
	//	}
	//}


	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
	{
		const FVector ItemLocation = GetItemLocation(QueryInstance, It.GetIndex());

		//bool bNear = false; 

		float Distance = FVector::Dist(ItemLocation, TargetLocation);

		float NormalizedScore = FMath::Clamp(Distance / MaxDistance, 0.0f, 1.0f);
		It.SetScore(TestPurpose, FilterType, Distance, 0.0f, 1.0f);
	}

	//// 제외할 아이템을 제외하고 나머지 아이템에 대해 점수와 필터 적용
	//for (int32 Index = 0; Index < QueryInstance.Items.Num(); ++Index)
	//{
	//	if (Index == ExcludedItemIndex)
	//	{
	//		// 제외된 아이템은 필터링: 점수를 최소로 설정 (예: -1)
	//		//QueryInstance.Items[Index] = -1.0f;
	//	}
	//	else
	//	{
	//		//FVector ItemLocation = GetItemLocation(QueryInstance, Index);
	//		FVector ItemLocation = QueryInstance.GetItemAsLocation(Index);
	//		float Distance = FVector::Dist(ItemLocation, TargetLocation);


	//		FLog::Print("Index " + FString::FromInt(Index) + "Distance  " + FString::FromInt(Distance), 2400);

	//		// 거리 계산 후 점수 설정
	//		if (Distance < MaxDistance)
	//		{
	//			QueryInstance.Items[Index] = Distance;
	//		}
	//		else
	//		{
	//			// 필터링: 조건을 벗어난 아이템은 점수를 낮게 설정
	//			QueryInstance.Items[Index] = -1.0f;
	//		}
	//	}
	//}


}

//ACharacter* character = Cast<ACharacter>(QueryInstance.Owner);
//CheckNull(character);
//FVector charLocation = character->GetActorLocation();
//FVector itemLocation = QueryInstance.GetItemAsLocation(0);

//for (int32 Index = 0; Index < QueryInstance.Items.Num(); Index++)
//{
//	float Distance = FVector::Dist(charLocation, itemLocation);
//	// 점수 부여
//	QueryInstance[Index] = Distance;
//}