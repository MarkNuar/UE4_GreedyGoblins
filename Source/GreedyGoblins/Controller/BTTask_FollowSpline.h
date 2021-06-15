// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "GreedyGoblins/Pawns/Enemies/LightHouse/LightHouse.h"

#include "BTTask_FollowSpline.generated.h"

/**
 * 
 */
UCLASS()
class GREEDYGOBLINS_API UBTTask_FollowSpline : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_FollowSpline();
	
protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	float InterpStep;

	float TimeInterp(float DeltaTime);

	float AlphaMovement = 0.0f;
	bool bIsAlphaIncreasing = true;

	UPROPERTY()
	ALightHouse* LightHouse;

	UPROPERTY()
	USceneComponent* PatrolTargetTransform;

	UPROPERTY()
	USplineComponent* SplineComponent;
};
