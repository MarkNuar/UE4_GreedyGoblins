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

public:
	bool GetCanGetSailKey() const;
	
	void SetCanGetSailKey();
	
	bool GetEnragedMode() const
	{
		return EnragedMode;
	}

	void UpdateSailKeyOwner(APlayerState* PlayerWithSailKeyParam);

	bool HasSailKey(APlayerState* PlayerState) const;
	
private:
	FTimerHandle SailKeyTimerHandle;
	
	UPROPERTY()
	APlayerState* PlayerWithSailKey = nullptr;

	UPROPERTY()
	APlayerState* OldPlayerWithSailKey = nullptr;

	UPROPERTY(EditAnywhere)
	float SailKeyHitDelay = 2.0f;
	
	UPROPERTY(Replicated)
	bool EnragedMode = false;

	bool CanGetSailKey = true;

	void StartSailKeyHitDelay();

	void UpdatePearlShield() const;
	
};
