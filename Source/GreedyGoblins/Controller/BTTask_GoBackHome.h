// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "GreedyGoblins/Pawns/Enemies/LightHouse/LightHouse.h"

#include "BTTask_GoBackHome.generated.h"

/**
 * 
 */
UCLASS()
class GREEDYGOBLINS_API UBTTask_GoBackHome : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_GoBackHome();

protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	FVector HomeSplinePosition;

	FVector StartPatrolTargetPosition;
	
	UPROPERTY()
	UCapsuleComponent* PatrolTargetComponent;

	float LerpStep;
	float LerpRatio = 0.f;
	
	UPROPERTY()
	ALightHouse* LightHouse; 
};


