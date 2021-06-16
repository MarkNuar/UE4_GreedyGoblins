// Fill out your copyright notice in the Description page of Project Settings.


#include "GreedyGoblinsGameState.h"

#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Pawns/Boat/Boat.h"
#define ECC_PLAYER_WITH_SAIL_KEY ECC_GameTraceChannel1

void AGreedyGoblinsGameState::UpdateSailKeyOwner(APlayerState* OldPlayerWithSailKeyParam, APlayerState* PlayerWithSailKeyParam)
{
	PlayerWithSailKey = PlayerWithSailKeyParam;	
	OldPlayerWithSailKey = OldPlayerWithSailKeyParam;
	StartSailKeyHitDelay();

	//if somebody took the sail key
	if(PlayerWithSailKey != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player number %d stole the sail key"), PlayerWithSailKey->GetPlayerId());
		ABoat* BoatWithSailKey = Cast<ABoat>(PlayerWithSailKey->GetPawn());
		if(!ensure(BoatWithSailKey != nullptr)) return;
		BoatWithSailKey->SetShowLightCylinder(true);
		EnragedMode = true;

		// if that somebody, stole it from someone else
		if(OldPlayerWithSailKey != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Player number %d has lost the sail key"), OldPlayerWithSailKey->GetPlayerId());
			ABoat* OldBoatWithSailKey = Cast<ABoat>(OldPlayerWithSailKey->GetPawn());
			if(!ensure(OldBoatWithSailKey != nullptr)) return;
			OldBoatWithSailKey->SetShowLightCylinder(false);
		}
	}
	// if the sail key was lost by being caught in a lighthouse light cone
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("Player number %d has lost the sail key"), OldPlayerWithSailKey->GetPlayerId());
		ABoat* OldBoatWithSailKey = Cast<ABoat>(OldPlayerWithSailKey->GetPawn());
		if(!ensure(OldBoatWithSailKey != nullptr)) return;
		OldBoatWithSailKey->SetShowLightCylinder(false);
		OldBoatWithSailKey = nullptr;
		
		//TODO SPAWN ACTOR (GENERATE) NEW SAIL OF KEY
	}

	UpdatePearlShield();
}

bool AGreedyGoblinsGameState::HasSailKey(APlayerState* PlayerState) const
{
	return PlayerWithSailKey == PlayerState;
}

void AGreedyGoblinsGameState::StartSailKeyHitDelay()
{
	if(!GetWorldTimerManager().IsTimerActive(SailKeyTimerHandle))
	{
		CanGetSailKey = false;
		GetWorldTimerManager().SetTimer(SailKeyTimerHandle, this, &AGreedyGoblinsGameState::SetCanGetSailKey,SailKeyHitDelay);
	}
}

void AGreedyGoblinsGameState::UpdatePearlShield() const
{
	//if somebody took the sail key
	if(PlayerWithSailKey != nullptr)
	{
		ABoat* BoatWithSailKey = Cast<ABoat>(PlayerWithSailKey->GetPawn());
		if(!ensure(BoatWithSailKey != nullptr)) return;
		BoatWithSailKey->BoxCollider->SetCollisionObjectType(ECC_PLAYER_WITH_SAIL_KEY);

		// if that somebody, stole it from someone else
		if(OldPlayerWithSailKey != nullptr)
		{
			ABoat* OldBoatWithSailKey = Cast<ABoat>(OldPlayerWithSailKey->GetPawn());
			if(!ensure(OldBoatWithSailKey != nullptr)) return;
			OldBoatWithSailKey->BoxCollider->SetCollisionObjectType(ECC_WorldDynamic);
		}
	}
	// if the sail key was lost by being caught in a lighthouse light cone
	else
	{
		ABoat* OldBoatWithSailKey = Cast<ABoat>(OldPlayerWithSailKey->GetPawn());
		if(!ensure(OldBoatWithSailKey != nullptr)) return;
		OldBoatWithSailKey->BoxCollider->SetCollisionObjectType(ECC_WorldDynamic);
	}
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
