// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "GreedyGoblinsGameState.generated.h"

/**
 * 
 */
UCLASS()
class GREEDYGOBLINS_API AGreedyGoblinsGameState : public AGameStateBase
{
	GENERATED_BODY()


private:
	
	UPROPERTY(Replicated)
	bool EnragedMode = false;

	UPROPERTY()
	APlayerState* PlayerWithSailKey = nullptr;

public:
	
	bool GetEnragedMode() const
	{
		return EnragedMode;
	}

	void UpdateSailKeyOwner(APlayerState* PlayerWithSailKeyParam);
};
