// Fill out your copyright notice in the Description page of Project Settings.


#include "ATest.h"

#include "GreedyGoblins/Pawns/Boat/Boat.h"

// Sets default values
AATest::AATest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(FName("TriggerVolume"));
	if(!ensure(TriggerVolume != nullptr)) return;

	RootComponent = TriggerVolume;

	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AATest::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AATest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AATest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AATest::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//if(OtherActor->GetClass()->IsChildOf(ABoat::StaticClass()))
	{
		UE_LOG(LogActor, Warning, TEXT("Overlapping with %s"), *this->GetName());
	}
}
