// Fill out your copyright notice in the Description page of Project Settings.


#include "SailKey.h"

#include "GreedyGoblins/Pawns/Boat/Boat.h"

// Sets default values
ASailKey::ASailKey()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(FName("TriggerVolume"));
	if (!ensure(TriggerVolume != nullptr)) return;
	SailMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("SailMesh"));
	if (!ensure(SailMesh != nullptr)) return;
	
    SailMesh->SetupAttachment(TriggerVolume);
	
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &ASailKey::OnOverlapBegin);
}

// Called when the game starts or when spawned
void ASailKey::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASailKey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASailKey::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	ABoat* BoatWithSailKey = Cast<ABoat>(OtherActor);
	BoatWithSailKey->SetHasSailKey(true);
	
	if(PearlOfDestiny == nullptr) return;
	UStaticMeshComponent* ShieldMesh = PearlOfDestiny->GetShieldMesh();
	if(ShieldMesh == nullptr) return;
	ShieldMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	this->Destroy();
}