#include "Triggers/GameStartTrigger.h"
#include "Global.h"
#include "Components/ShapeComponent.h"

#include "Characters/CPlayer.h"
#include "GameInstances/CGameInstance.h"
#include "GameInstances/CGameManager.h"


AGameStartTrigger::AGameStartTrigger()
{
	FHelpers::CreateComponent<UShapeComponent>(this, &TriggerShape, "TriggerShape");
}

void AGameStartTrigger::BeginPlay()
{
	if (!!TriggerShapeClass)
	{
		TriggerShape = NewObject<UShapeComponent>(this, TriggerShapeClass);

		if (TriggerShape != nullptr)
		{
			TriggerShape->RegisterComponent();
			TriggerShape->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		}
	}

	CheckNull(TriggerShape);
	// Trigger의 충돌 처리
	TriggerShape->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerShape->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	TriggerShape->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerShape->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	TriggerShape->OnComponentBeginOverlap.AddDynamic(this, &AGameStartTrigger::OnTriggerEnter);
	TriggerShape->OnComponentEndOverlap.AddDynamic(this, &AGameStartTrigger::OnTriggerExit);

	Super::BeginPlay();
}

void AGameStartTrigger::OnTriggerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckNull(OtherActor);

	ACPlayer* player = Cast<ACPlayer>(OtherActor);
	CheckNull(player);

	FLog::Log("Trigger In");
	UCGameInstance* instance = Cast<UCGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	CheckNull(instance); 
	CheckNull(instance->GameManager);
	
	instance->GameManager->StartGame();
}

void AGameStartTrigger::OnTriggerExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckNull(OtherActor);

	ACPlayer* player = Cast<ACPlayer>(OtherActor);
	CheckNull(player);

	FLog::Log("Trigger Out");
}


