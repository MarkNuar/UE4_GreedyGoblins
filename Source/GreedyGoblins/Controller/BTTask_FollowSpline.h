// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"

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

};
