// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GoBackHome.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_GoBackHome::UBTTask_GoBackHome()
{
	NodeName = TEXT("Go Back Home");

	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_GoBackHome::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if(OwnerComp.GetAIOwner() == nullptr) return EBTNodeResult::Failed; // If there is not AI controller
	LightHouse = Cast<ALightHouse>(OwnerComp.GetAIOwner()->GetPawn());
	if(LightHouse == nullptr) return EBTNodeResult::Failed;

	if(LightHouse->GetPatrolTargetComponent() == nullptr) return EBTNodeResult::Failed;
	PatrolTargetComponent = LightHouse->GetPatrolTargetComponent();
	
	float Distance = (PatrolTargetComponent->GetComponentLocation() - HomeSplinePosition).Size();
	float Time =  Distance / (LightHouse->GetLightSpeed() * 100.0f);
	LerpStep = 1 / Time;

	
	HomeSplinePosition = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());
	return EBTNodeResult::InProgress;
	
}

void UBTTask_GoBackHome::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{

	LerpRatio += LerpStep * DeltaSeconds;
	FVector PatrolTargetPosition = FMath::LerpStable(PatrolTargetComponent->GetComponentLocation(), HomeSplinePosition, LerpRatio);

	PatrolTargetComponent->SetWorldLocation(PatrolTargetPosition);
	
	if(LerpRatio >= 1)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

}

