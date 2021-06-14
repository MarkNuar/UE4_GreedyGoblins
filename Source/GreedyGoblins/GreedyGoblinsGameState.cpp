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
	if(PlayerWithSailKey != nullptr)
	{
		OldPlayerWithSailKey = PlayerWithSailKey;
		UE_LOG(LogTemp, Warning, TEXT("Player number %d has lost the sail key"), OldPlayerWithSailKey->GetPlayerId());
	}
		
	PlayerWithSailKey = PlayerWithSailKeyParam;
	EnragedMode = true;
	StartSailKeyHitDelay();
	UpdatePearlShield();

	UE_LOG(LogTemp, Warning, TEXT("Player number %d stole the sail key"), PlayerWithSailKey->GetPlayerId());
	
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

void AGreedyGoblinsGameState::UpdatePearlShield()
{
	ABoat* BoatWithSailKey = Cast<ABoat>(PlayerWithSailKey->GetPawn());
	if(!ensure(BoatWithSailKey != nullptr)) return;

	BoatWithSailKey->BoxCollider->SetCollisionProfileName("PlayerWithSailKey");
	
	if(OldPlayerWithSailKey != nullptr)
	{
		ABoat* OldBoatWithSailKey = Cast<ABoat>(OldPlayerWithSailKey->GetPawn());
		if(!ensure(OldBoatWithSailKey != nullptr)) return;
		OldBoatWithSailKey->BoxCollider->SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);
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
