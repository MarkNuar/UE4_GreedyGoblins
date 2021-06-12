// Fill out your copyright notice in the Description page of Project Settings.


#include "GreedyGoblinsGameState.h"

#include "GameFramework/PlayerState.h"
#include "Pawns/Boat/Boat.h"

void AGreedyGoblinsGameState::SetPlayerWithSailKey(APlayerState* PlayerWithSailKeyParam)
{
	this->PlayerWithSailKey = PlayerWithSailKeyParam;
	EnragedMode = true;
	ABoat* BoatWithSailKey = Cast<ABoat>(PlayerWithSailKey->GetPawn());

	UE_LOG(LogTemp, Warning, TEXT("%d"), PlayerWithSailKey->GetPlayerId());
	//TODO BoatWithSailKey->SetBoatLightCylinder();
}
