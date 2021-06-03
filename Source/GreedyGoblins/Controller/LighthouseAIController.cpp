// Fill out your copyright notice in the Description page of Project Settings.


#include "LighthouseAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"


void ALighthouseAIController::BeginPlay()
{
	Super::BeginPlay();

	if(AIBehavior != nullptr)
	{
		RunBehaviorTree(AIBehavior);
		// APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0); // gets the single player playing
		// GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"),PlayerPawn->GetActorLocation());
		// APawn* AIPawn = GetPawn(); // gets the pawn that this ai controller is controlling
		// GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"),AIPawn->GetActorLocation());
	}
}


void ALighthouseAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}