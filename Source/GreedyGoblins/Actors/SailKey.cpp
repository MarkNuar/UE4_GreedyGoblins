// Fill out your copyright notice in the Description page of Project Settings.


#include "SailKey.h"

#include "GreedyGoblins/GreedyGoblinsGameState.h"
#include "GreedyGoblins/Pawns/Boat/Boat.h"

// Sets default values
ASailKey::ASailKey()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(FName("TriggerVolume"));
	if (!ensure(TriggerVolume != nullptr)) return;
	SailMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("SailMesh"));
	if (!ensure(SailMesh != nullptr)) return;

	RootComponent = TriggerVolume;
    SailMesh->SetupAttachment(TriggerVolume);

	if(HasAuthority())
	{
		TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &ASailKey::OnOverlapBegin);
	}
}

// Called when the game starts or when spawned
void ASailKey::BeginPlay()
{
	Super::BeginPlay();
	GreedyGoblinsGameState = Cast<AGreedyGoblinsGameState>(GetWorld()->GetGameState());
	if (!ensure(GreedyGoblinsGameState != nullptr)) return;
}

// Called every frame
void ASailKey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// if(GreedyGoblinsGameState->GetEnragedMode() && !HasAuthority())
	//	this->Destroy();
}

void ASailKey::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherComp->GetClass()->IsChildOf(UBoxComponent::StaticClass()))
	{
		ABoat* BoatWithSailKey = Cast<ABoat>(OtherActor);
		if (!ensure(BoatWithSailKey != nullptr)) return;
	
		APlayerState* PlayerStateWithSailKey = BoatWithSailKey->GetPlayerState();
		if (!ensure(PlayerStateWithSailKey != nullptr)) return;
	
		GreedyGoblinsGameState = Cast<AGreedyGoblinsGameState>(GetWorld()->GetGameState());
		if (!ensure(GreedyGoblinsGameState != nullptr)) return;
	
		GreedyGoblinsGameState->UpdateSailKeyOwner(nullptr,PlayerStateWithSailKey);
	
		this->Destroy();
	}
}
