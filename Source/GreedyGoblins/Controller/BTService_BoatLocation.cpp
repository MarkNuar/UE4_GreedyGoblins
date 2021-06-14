// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_BoatLocation.h"

#include "BehaviorTree/BlackboardComponent.h"


UBTService_BoatLocation::UBTService_BoatLocation()
{
	NodeName = "Update Boat Location";
}


void UBTService_BoatLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* PlayerPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("BoatToChase")));
	if(PlayerPawn == nullptr)
		return;

	OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), PlayerPawn->GetActorLocation());
}