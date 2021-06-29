// Fill out your copyright notice in the Description page of Project Settings.


#include "GreedyGoblinsGameState.h"

#include "GreedyGoblinsGameInstance.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Pawns/Boat/Boat.h"
#include "Pawns/Enemies/LightHouse/LightHouse.h"
#define ECC_PLAYER_WITH_SAIL_KEY ECC_GameTraceChannel1

AGreedyGoblinsGameState::AGreedyGoblinsGameState()
{
	const ConstructorHelpers::FClassFinder<ASailKey> SailKeyBPClass(TEXT("/Game/Blueprint/Actors/BP_SailKey"));
	if(!ensure(SailKeyBPClass.Class!=nullptr)) return;
	SailKeyClass = SailKeyBPClass.Class;

	// Fetch all Lighthouses
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALightHouse::StaticClass(), LightHouses);
}

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
		bIsInEnragedMode = true;

		SailKeyRecoveryLocation = BoatWithSailKey->GetActorLocation(); // used for handling a player with sail key disconnection
		
		// if that somebody, stole it from someone else
		if(OldPlayerWithSailKey != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Player number %d has lost the sail key"), OldPlayerWithSailKey->GetPlayerId());
			ABoat* OldBoatWithSailKey = Cast<ABoat>(OldPlayerWithSailKey->GetPawn());
			if(!ensure(OldBoatWithSailKey != nullptr)) return;
			OldBoatWithSailKey->SetShowLightCylinder(false);
		}
		else
		{
			SetEnragedModeToAllLighthouses();
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
		bIsInEnragedMode = false;
		
		// todo: set enrage mode false to all lighthouses
		SetEnragedModeToAllLighthouses();
	}

	UpdatePearlShield();
}

void AGreedyGoblinsGameState::DropSailKeyAtLocation(FVector Location)
{
	UpdateSailKeyOwner(PlayerWithSailKey, nullptr);
	GetWorld()->SpawnActor<ASailKey>(SailKeyClass, Location, FRotator(0, 0, 0));
}

bool AGreedyGoblinsGameState::HasSailKey(APlayerState* PlayerState) const
{
	return PlayerWithSailKey == PlayerState;
}

void AGreedyGoblinsGameState::StartSailKeyHitDelay()
{
	if(!GetWorldTimerManager().IsTimerActive(SailKeyTimerHandle))
	{
		bCanGetSailKey = false;
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
	DOREPLIFETIME(AGreedyGoblinsGameState, bIsInEnragedMode);
	DOREPLIFETIME(AGreedyGoblinsGameState, bIsGameEnded);
}

void AGreedyGoblinsGameState::On_bIsGameEnded_Rep()
{
	ShowEndScreen();
}

void AGreedyGoblinsGameState::On_bIsInEnragedMode_Rep()
{
	SetEnragedModeToAllLighthouses();
}

bool AGreedyGoblinsGameState::GetCanGetSailKey() const
{
	return bCanGetSailKey;
}

void AGreedyGoblinsGameState::SetCanGetSailKey()
{
	bCanGetSailKey = true;
}

void AGreedyGoblinsGameState::SetEnragedModeToAllLighthouses()
{
	for(AActor* Actor : LightHouses)
	{
		ALightHouse* LightHouse = Cast<ALightHouse>(Actor);
		if(!ensure(LightHouse)) return;
		LightHouse->SetIsInEnragedMode(bIsInEnragedMode);
	}
}

void AGreedyGoblinsGameState::ShowEndScreen()
{
	UGreedyGoblinsGameInstance* GameInstance = Cast<UGreedyGoblinsGameInstance>(GetGameInstance());
	if(!ensure(GameInstance)) return;
	GameInstance->LoadEndGameMenu();
}

void AGreedyGoblinsGameState::HandlePlayerDisconnection(APlayerState* Exiting)
{
	if(PlayerWithSailKey == Exiting)
	{
		DropSailKeyAtLocation(SailKeyRecoveryLocation);
		PlayerWithSailKey = nullptr;
		bIsInEnragedMode = false;
		SetEnragedModeToAllLighthouses();
	}
	else if (OldPlayerWithSailKey == Exiting)
	{
		OldPlayerWithSailKey = nullptr;
	}
}

