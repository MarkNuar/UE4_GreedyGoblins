// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "GreedyGoblins/Pawns/Enemies/LightHouse/LightHouse.h"

#include "BTTask_ChaseBoat.generated.h"

/**
 * 
 */
UCLASS()
class GREEDYGOBLINS_API UBTTask_ChaseBoat : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_ChaseBoat();

private:

	UPROPERTY()
	UCapsuleComponent* PatrolTargetComponent;

	UPROPERTY()
	ALightHouse* LightHouse;

	FVector BaseLocation;
	float MaxPatrolTargetDistance;

protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
