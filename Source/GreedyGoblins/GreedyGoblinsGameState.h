// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "DrawDebugHelpers.h"
#include "GameFramework/GameState.h"
#include "GreedyGoblinsGameState.generated.h"

class ASailKey;
/**
 * 
 */
UCLASS()
class GREEDYGOBLINS_API AGreedyGoblinsGameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	AGreedyGoblinsGameState();
	
	bool GetCanGetSailKey() const;
	
	void SetCanGetSailKey();
	
	bool GetEnragedMode() const
	{
		return bIsInEnragedMode;
	}

	void SetIsGameEnded(bool bIsGameEndedParam)
	{
		this->bIsGameEnded = bIsGameEndedParam;
	}
	
	void UpdateSailKeyOwner(APlayerState* OldPlayerWithSailKeyParam, APlayerState* PlayerWithSailKeyParam);

	void DropSailKeyAtLocation(FVector Location);
	
	bool HasSailKey(APlayerState* PlayerState) const;
	
	void ShowEndScreen();

	void HandlePlayerDisconnection(APlayerState* Exiting);
	
private:
	FTimerHandle SailKeyTimerHandle;
	
	UPROPERTY()
	APlayerState* PlayerWithSailKey = nullptr;

	UPROPERTY()
	APlayerState* OldPlayerWithSailKey = nullptr;

	TSubclassOf<ASailKey> SailKeyClass;
	
	UPROPERTY(EditAnywhere)
	float SailKeyHitDelay = 2.0f;
	
	UPROPERTY(ReplicatedUsing = On_bIsInEnragedMode_Rep)
	bool bIsInEnragedMode = false;

	UFUNCTION()
	void On_bIsInEnragedMode_Rep();

	UFUNCTION()
	void On_bIsGameEnded_Rep();
	
	bool bCanGetSailKey = true;

	UPROPERTY(ReplicatedUsing = On_bIsGameEnded_Rep)
	bool bIsGameEnded = false;
	
	void StartSailKeyHitDelay();

	void UpdatePearlShield() const;

	void SetEnragedModeToAllLighthouses();

	FVector SailKeyRecoveryLocation;

	UPROPERTY()
	TArray<AActor*> LightHouses;

};
