// Fill out your copyright notice in the Description page of Project Settings.


#include "GreedyGoblinsGameState.h"

#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Pawns/Boat/Boat.h"
#define ECC_PlayerWithSailKey ECC_GameTraceChannel1

void AGreedyGoblinsGameState::UpdateSailKeyOwner(APlayerState* PlayerWithSailKeyParam)
{
	if(PlayerWithSailKey != nullptr)
	{
		OldPlayerWithSailKey = PlayerWithSailKey;
		UE_LOG(LogTemp, Warning, TEXT("Player number %d has lost the sail key"), OldPlayerWithSailKey->GetPlayerId());

		ABoat* OldBoatWithSailKey = Cast<ABoat>(OldPlayerWithSailKey->GetPawn());
		if(!ensure(OldBoatWithSailKey != nullptr)) return;

		OldBoatWithSailKey->SetShowLightCylinder(false);
	}
		
	PlayerWithSailKey = PlayerWithSailKeyParam;
	EnragedMode = true;
	StartSailKeyHitDelay();
	UpdatePearlShield();

	UE_LOG(LogTemp, Warning, TEXT("Player number %d stole the sail key"), PlayerWithSailKey->GetPlayerId());
	
	//TODO BoatWithSailKey->Mesh->EnableLightCylinder();
	
	ABoat* BoatWithSailKey = Cast<ABoat>(PlayerWithSailKey->GetPawn());
	if(!ensure(BoatWithSailKey != nullptr)) return;

	BoatWithSailKey->SetShowLightCylinder(true);
}

bool AGreedyGoblinsGameState::HasSailKey(APlayerState* PlayerState)
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

void AGreedyGoblinsGameState::UpdatePearlShield()
{
	ABoat* BoatWithSailKey = Cast<ABoat>(PlayerWithSailKey->GetPawn());
	if(!ensure(BoatWithSailKey != nullptr)) return;

	BoatWithSailKey->BoxCollider->SetCollisionObjectType(ECC_PlayerWithSailKey);
	
	if(OldPlayerWithSailKey != nullptr)
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
