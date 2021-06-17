// Copyright Epic Games, Inc. All Rights Reserved.


#include "GreedyGoblinsGameModeBase.h"

#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

AGreedyGoblinsGameModeBase::AGreedyGoblinsGameModeBase()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), FreePlayerStarts);
}

AActor* AGreedyGoblinsGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{	
	int Index = FMath::RandRange(0, FreePlayerStarts.Num()-1);
	AActor* PlayerStart = FreePlayerStarts[Index];
	FreePlayerStarts.RemoveAt(Index);
	return PlayerStart;
}
