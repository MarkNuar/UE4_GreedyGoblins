// Copyright Epic Games, Inc. All Rights Reserved.


#include "GreedyGoblinsGameModeBase.h"

#include "GameFramework/PlayerState.h"
#include "GameFramework/PlayerStart.h"
#include "GreedyGoblins/Actors/SailKey.h"
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

void AGreedyGoblinsGameModeBase::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	
	AGreedyGoblinsGameState* GreedyGoblinsGameState = Cast<AGreedyGoblinsGameState>(GetWorld()->GetGameState());
	if (!ensure(GreedyGoblinsGameState != nullptr)) return;

	APlayerState* PlayerState = Exiting->GetPlayerState<APlayerState>();
	if (!ensure(PlayerState != nullptr)) return;
	
	GreedyGoblinsGameState->HandlePlayerDisconnection(PlayerState);
}
