// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GoBackHome.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_GoBackHome::UBTTask_GoBackHome()
{
	NodeName = TEXT("Go Back Home");
	bNotifyTick = true;
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTask_GoBackHome::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if(OwnerComp.GetAIOwner() == nullptr) return EBTNodeResult::Failed; // If there is not AI controller
	LightHouse = Cast<ALightHouse>(OwnerComp.GetAIOwner()->GetPawn());
	if(LightHouse == nullptr) return EBTNodeResult::Failed;

	if(LightHouse->GetPatrolTargetTransform() == nullptr) return EBTNodeResult::Failed;
	PatrolTargetTransform = LightHouse->GetPatrolTargetTransform();

	HomeSplinePosition = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());
	StartPatrolTargetPosition = PatrolTargetTransform->GetComponentLocation();

	const float Distance = (PatrolTargetTransform->GetComponentLocation() - HomeSplinePosition).Size();
	const float Time =  Distance / (LightHouse->GetLightSpeed() * 100.0f);
	LerpStep = 1 / Time;
	LerpRatio = 0.0f;
	
	PrevLightSpeed = LightHouse->GetLightSpeed();
	
	return EBTNodeResult::InProgress;
	
}

void UBTTask_GoBackHome::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if(LightHouse->GetLightSpeed() != PrevLightSpeed)
	{
		ExecuteTask(OwnerComp, NodeMemory); // let's pray
	}
	else
	{
		LerpRatio += LerpStep * DeltaSeconds;

		if(LerpRatio >= 1)
		{
			FName SplineHome = TEXT("HomeSplinePosition");
			OwnerComp.GetBlackboardComponent()->ClearValue(SplineHome);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
        
        FVector PatrolTargetPosition = FMath::LerpStable(StartPatrolTargetPosition, HomeSplinePosition, LerpRatio);
    
        PatrolTargetTransform->SetWorldLocation(PatrolTargetPosition);
        
		PrevLightSpeed = LightHouse->GetLightSpeed();
	}
}

