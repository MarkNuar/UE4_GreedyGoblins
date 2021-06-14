// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ChaseBoat.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GreedyGoblins/Pawns/Enemies/LightHouse/LightHouse.h"

UBTTask_ChaseBoat::UBTTask_ChaseBoat()
{
	NodeName = "Chase Boat";
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_ChaseBoat::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	LightHouse = Cast<ALightHouse>(OwnerComp.GetAIOwner()->GetPawn());
	if(!ensure(LightHouse != nullptr)) return EBTNodeResult::Failed;
	
	BaseLocation = LightHouse->GetActorLocation();
	MaxPatrolTargetDistance = LightHouse->GetMaxPatrolTargetDistance();
	PatrolTargetComponent = LightHouse->GetPatrolTargetComponent();
	
	return EBTNodeResult::InProgress;
}

void UBTTask_ChaseBoat::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FVector BoatLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());

	if((BoatLocation - BaseLocation).Size() > MaxPatrolTargetDistance * 100)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Aborted);
		return;
	}
	
	FVector MoveDirection = FVector::VectorPlaneProject(BoatLocation - PatrolTargetComponent->GetComponentLocation(), FVector::ZAxisVector).GetSafeNormal();
	FVector Translation = MoveDirection * 100 * LightHouse->GetLightSpeed() * DeltaSeconds;
	PatrolTargetComponent->AddWorldOffset(Translation);
}
