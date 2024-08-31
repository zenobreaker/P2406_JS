#include "CActorComponent_Test.h"

UCActorComponent_Test::UCActorComponent_Test()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UCActorComponent_Test::BeginPlay()
{
	Super::BeginPlay();

	
	
}


// Called every frame
void UCActorComponent_Test::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

