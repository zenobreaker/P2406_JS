#include "BehaviorTree/EQT/CEnvQueryTest_ExcludeCurLocation.h"
#include "Global.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_VectorBase.h"
#include "GameFramework/Character.h"

UCEnvQueryTest_ExcludeCurLocation::UCEnvQueryTest_ExcludeCurLocation()
{
	Cost = EEnvTestCost::Low;
	ValidItemType = UEnvQueryItemType_VectorBase::StaticClass(); // ���� ������ Ÿ�� ����
}

void UCEnvQueryTest_ExcludeCurLocation::RunTest(FEnvQueryInstance& QueryInstance) const
{
	bool bWantNear = BoolValue.GetValue();

	// ������ �������� �ε����� ������ ����
	int32 ExcludedItemIndex = -1;
	float ClosestDistance = FLT_MAX; // ���� ����� �������� �Ÿ�

	//// ���� �������� ���� ������ �������� ã�� ���� (��: ����� �Ÿ��� ������)
	//for (int32 Index = 0; Index < QueryInstance.Items.Num(); ++Index)
	//{
	//	// �������� ��ġ�� ������
	//	FVector ItemLocation = GetItemLocation(QueryInstance, Index);
	//	float Distance = FVector::Dist(ItemLocation, TargetLocation);

	//	// ���� ����� �������� ������ �ε����� ����
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

	//// ������ �������� �����ϰ� ������ �����ۿ� ���� ������ ���� ����
	//for (int32 Index = 0; Index < QueryInstance.Items.Num(); ++Index)
	//{
	//	if (Index == ExcludedItemIndex)
	//	{
	//		// ���ܵ� �������� ���͸�: ������ �ּҷ� ���� (��: -1)
	//		//QueryInstance.Items[Index] = -1.0f;
	//	}
	//	else
	//	{
	//		//FVector ItemLocation = GetItemLocation(QueryInstance, Index);
	//		FVector ItemLocation = QueryInstance.GetItemAsLocation(Index);
	//		float Distance = FVector::Dist(ItemLocation, TargetLocation);


	//		FLog::Print("Index " + FString::FromInt(Index) + "Distance  " + FString::FromInt(Distance), 2400);

	//		// �Ÿ� ��� �� ���� ����
	//		if (Distance < MaxDistance)
	//		{
	//			QueryInstance.Items[Index] = Distance;
	//		}
	//		else
	//		{
	//			// ���͸�: ������ ��� �������� ������ ���� ����
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
//	// ���� �ο�
//	QueryInstance[Index] = Distance;
//}