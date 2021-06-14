// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_BoatLocation.generated.h"

/**
 * 
 */
UCLASS()
class GREEDYGOBLINS_API UBTService_BoatLocation : public UBTService_BlackboardBase
{
	GENERATED_BODY()

	public:
	UBTService_BoatLocation();
	
	protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
