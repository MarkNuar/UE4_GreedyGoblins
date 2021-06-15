// Fill out your copyright notice in the Description page of Project Settings.


#include "LighthouseAIController.h"
#include "Kismet/GameplayStatics.h"

void ALighthouseAIController::BeginPlay()
{
	Super::BeginPlay();
	
	if(AIBehavior != nullptr && HasAuthority())
	{
		RunBehaviorTree(AIBehavior);
	}
}

void ALighthouseAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}