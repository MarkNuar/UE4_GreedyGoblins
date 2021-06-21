// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GreedyGoblinsGameModeBase.h"
#include "LobbyGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class GREEDYGOBLINS_API ALobbyGameModeBase : public AGreedyGoblinsGameModeBase
{
	GENERATED_BODY()
public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	
	virtual void Logout(AController* Exiting) override;
	
private:

	uint32 NumberOfPlayers = 0;

	UPROPERTY(EditAnywhere)
	uint32 MinNumberOfPlayers = 3;

	UPROPERTY(EditAnywhere)
	uint32 MaxNumberOfPlayers = 8;
	
	void StartGame();

	FTimerHandle GameStartTimer;
	
};
