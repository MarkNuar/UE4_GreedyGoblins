// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_BoatLocationIfHeard.generated.h"

class ALightHouse;
/**
 * 
 */
UCLASS()
class GREEDYGOBLINS_API UBTService_BoatLocationIfHeard : public UBTService_BlackboardBase
{
	GENERATED_BODY()


	public:
	UBTService_BoatLocationIfHeard();
	
	protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	private:
	void Clear(UBehaviorTreeComponent& OwnerComp, FName SelectedBlackboardKey, ALightHouse* LightHouse);
};
