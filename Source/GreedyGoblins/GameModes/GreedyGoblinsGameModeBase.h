// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "GreedyGoblinsGameModeBase.generated.h"

class ASailKey;
/**
 * 
 */
UCLASS()
class GREEDYGOBLINS_API AGreedyGoblinsGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	AGreedyGoblinsGameModeBase();

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	
private:
	UPROPERTY()
	TArray<AActor*> FreePlayerStarts;
};

