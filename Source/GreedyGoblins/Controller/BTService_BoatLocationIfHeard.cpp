// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_BoatLocationIfHeard.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GreedyGoblins/Pawns/Enemies/LightHouse/LightHouse.h"

UBTService_BoatLocationIfHeard::UBTService_BoatLocationIfHeard()
{
	NodeName = "Update Boat Location If Heard";
}


void UBTService_BoatLocationIfHeard::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIController = OwnerComp.GetAIOwner(); 
	if(!ensure(AIController != nullptr)) return;

	APawn* Pawn = AIController->GetPawn();
	if(!ensure(Pawn != nullptr)) return;
	
	ALightHouse* LightHouse = Cast<ALightHouse>(Pawn);
	if(!ensure(LightHouse != nullptr)) return;

	ABoat* BoatToChase = LightHouse->GetBoatToChase();
	
	if(BoatToChase != nullptr)
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), BoatToChase);
	else
		OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey()); 
	
}

