// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FollowSpline.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GreedyGoblins/Pawns/Enemies/LightHouse/LightHouse.h"

UBTTask_FollowSpline::UBTTask_FollowSpline()
{
	NodeName = TEXT("Follow Spline");
	bNotifyTick = true;
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTask_FollowSpline::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if(OwnerComp.GetAIOwner() == nullptr) return EBTNodeResult::Failed; // If there is not AI controller
	LightHouse = Cast<ALightHouse>(OwnerComp.GetAIOwner()->GetPawn());
	SplineComponent = LightHouse->GetSplineComponent();
	PatrolTargetTransform = LightHouse->GetPatrolTargetTransform();
	if(LightHouse == nullptr) return EBTNodeResult::Failed;

	if(PatrolTargetTransform == nullptr) return EBTNodeResult::Failed;
	if(SplineComponent == nullptr) return EBTNodeResult::Failed;

	PatrolTargetTransform->SetWorldLocation(SplineComponent->GetLocationAtDistanceAlongSpline(AlphaMovement * SplineComponent->GetSplineLength(), ESplineCoordinateSpace::World));
	
	float Time = SplineComponent->GetSplineLength() / (LightHouse->GetLightSpeed() * 100.0f);
    
    InterpStep = 1 / Time;

	PrevLightSpeed = LightHouse->GetLightSpeed();
	
	return EBTNodeResult::InProgress;
}

void UBTTask_FollowSpline::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	// Get reference to the controlled Pawn and execute shoot function
	
	if(!ensure(SplineComponent)) UE_LOG(LogTemp, Warning, TEXT("%s not found"), *SplineComponent->GetName());
	if(!ensure(PatrolTargetTransform)) UE_LOG(LogTemp, Warning, TEXT("%s not found"), *PatrolTargetTransform->GetName());

	if(LightHouse->GetLightSpeed() != PrevLightSpeed)
	{
		ExecuteTask(OwnerComp, NodeMemory); // let's pray
	}
	else
	{
		float InterpPosition = TimeInterp(DeltaSeconds);
		PatrolTargetTransform->SetWorldLocation(SplineComponent->GetLocationAtDistanceAlongSpline(
			InterpPosition*SplineComponent->GetSplineLength(), ESplineCoordinateSpace::World
			));

		PrevLightSpeed = LightHouse->GetLightSpeed();
	}
}

float UBTTask_FollowSpline::TimeInterp(float DeltaTime){
	if(!SplineComponent->IsClosedLoop())
	{
		if(bIsAlphaIncreasing){
			AlphaMovement += InterpStep*DeltaTime;
			if(AlphaMovement>=1-KINDA_SMALL_NUMBER){
				bIsAlphaIncreasing = false;
			}
		}else{
			AlphaMovement -= InterpStep*DeltaTime;
			if(AlphaMovement<=KINDA_SMALL_NUMBER){
				bIsAlphaIncreasing = true;
			}
		}
	}
	else
	{
		AlphaMovement += InterpStep*DeltaTime;
		if(AlphaMovement>=1)
		{
			AlphaMovement-= 1.0f;
		}
	}
	return AlphaMovement;
}
