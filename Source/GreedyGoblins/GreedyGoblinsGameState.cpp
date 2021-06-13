// Fill out your copyright notice in the Description page of Project Settings.


#include "GreedyGoblinsGameState.h"

#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Pawns/Boat/Boat.h"

void AGreedyGoblinsGameState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PearlOfDestiny = Cast<APearlOfDestiny>(UGameplayStatics::GetActorOfClass(GetWorld(),APearlOfDestiny::StaticClass()));
	if(!ensure(PearlOfDestiny != nullptr)) UE_LOG(LogTemp, Error, TEXT("Pearl of Destiny not found!"));
}

void AGreedyGoblinsGameState::UpdateSailKeyOwner(APlayerState* PlayerWithSailKeyParam)
{
	this->PlayerWithSailKey = PlayerWithSailKeyParam;
	EnragedMode = true;
	StartSailKeyHitDelay();
	DisableShieldForSailKeyOwner();
	
	UE_LOG(LogTemp, Warning, TEXT("%d has the sail key"), PlayerWithSailKey->GetPlayerId());
	
	//TODO BoatWithSailKey->SetBoatLightCylinder();
}

bool AGreedyGoblinsGameState::HasSailKey(APlayerState* PlayerState)
{
	return PlayerWithSailKey == PlayerState;
}

void AGreedyGoblinsGameState::StartSailKeyHitDelay()
{
	if(!GetWorldTimerManager().IsTimerActive(SailKeyHitDelay))
	{
		CanGetSailKey = false;
		GetWorldTimerManager().SetTimer(SailKeyHitDelay, this, &AGreedyGoblinsGameState::SetCanGetSailKey,2);
	}
}

void AGreedyGoblinsGameState::DisableShieldForSailKeyOwner()
{
	ABoat* BoatWithSailKey = Cast<ABoat>(PlayerWithSailKey->GetPawn());
	if(!ensure(BoatWithSailKey != nullptr)) return;
	
	//TODO  PlayerWithSailKeyBoat->BoxCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1);
}

void AGreedyGoblinsGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGreedyGoblinsGameState, EnragedMode);
}

bool AGreedyGoblinsGameState::GetCanGetSailKey() const
{
	return CanGetSailKey;
}

void AGreedyGoblinsGameState::SetCanGetSailKey()
{
	CanGetSailKey = true;
}
