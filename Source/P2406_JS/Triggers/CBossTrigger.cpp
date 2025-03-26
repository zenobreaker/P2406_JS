#include "Triggers/CBossTrigger.h"
#include "Global.h"
#include "Components/ShapeComponent.h"

#include "Characters/CPlayer.h"
#include "Characters/CBoss_AI.h"
#include "Components/CHealthPointComponent.h"

ACBossTrigger::ACBossTrigger()
{
	FHelpers::CreateComponent<UShapeComponent>(this, &TriggerShape, "TriggerShape");
}

void ACBossTrigger::BeginPlay()
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

	TriggerShape->OnComponentBeginOverlap.AddDynamic(this, &ACBossTrigger::OnTriggerEnter);
	TriggerShape->OnComponentEndOverlap.AddDynamic(this, &ACBossTrigger::OnTriggerExit);

	Super::BeginPlay();
}

void ACBossTrigger::OnTriggerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	CheckNull(OtherActor); 

	ACPlayer* player = Cast<ACPlayer>(OtherActor); 
	CheckNull(player); 

	TArray<AActor*> find;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACBoss_AI::StaticClass(), find);

	for (AActor* actor : find)
	{
		ACBoss_AI* boss = Cast<ACBoss_AI>(actor);
		if (boss == nullptr)
			continue;

		UCHealthPointComponent* health = FHelpers::GetComponent<UCHealthPointComponent>(boss);

		if (health == nullptr)
			continue; 
		REGISTER_EVENT_WITH_REPLACE(health, OnHealthPointChanged, player, ACPlayer::UpdateBossGauge);
		REGISTER_EVENT_WITH_REPLACE(boss, OnCharacterDead, player, ACPlayer::HideBossGauge);
		
		player->UpdateBossGauge(health->GetHealth(), health->GetMaxHealth());
		break; 
	}

	player->VisibleBossGauge();
}

void ACBossTrigger::OnTriggerExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckNull(OtherActor);

	ACPlayer* player = Cast<ACPlayer>(OtherActor);
	CheckNull(player);

	player->HideBossGauge();
}


