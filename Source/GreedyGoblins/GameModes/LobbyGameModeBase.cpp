// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameModeBase.h"

#include "GreedyGoblins/GreedyGoblinsGameInstance.h"
#include "Kismet/GameplayStatics.h"

void ALobbyGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	++NumberOfPlayers;
	
	if(NumberOfPlayers == MaxNumberOfPlayers)
	{
		this->StartGame();
	}
	
	/*
	else if(NumberOfPlayers >= MinNumberOfPlayers)
	{
		GetWorldTimerManager().SetTimer(GameStartTimer, this, &ALobbyGameModeBase::StartGame, 10); // after 5 seconds calls StartGame function
		//TODO DISPLAY TIMER BEFORE START 
	}*/
	
}

void ALobbyGameModeBase::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	--NumberOfPlayers;
}

void ALobbyGameModeBase::StartGame()
{
	UGreedyGoblinsGameInstance* GameInstance = Cast<UGreedyGoblinsGameInstance>(GetGameInstance());
	if(GameInstance==nullptr) return;
	GameInstance->StartSession(); // Marks an online session as in progress (as opposed to being in lobby or pending), session closed to new players
	
	UWorld* World = GetWorld();
	if(!ensure(World!=nullptr)) return;

	bUseSeamlessTravel = true; // Enables seamless travel between lobby and game map
	World->ServerTravel("/Game/Levels/Maps/4_8_FriedArchipelagus?listen"); // when we reach 3 players, travel to the game
}

