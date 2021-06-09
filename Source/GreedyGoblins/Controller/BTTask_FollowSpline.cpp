// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FollowSpline.h"

UBTTask_FollowSpline::UBTTask_FollowSpline()
{
	NodeName = TEXT("Follow Spline");

	bNotifyTick = true;
	
}

EBTNodeResult::Type UBTTask_FollowSpline::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	UE_LOG(LogTemp, Warning, TEXT("Execute Task"));

	return EBTNodeResult::InProgress;
}

void UBTTask_FollowSpline::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	// Get reference to the controlled Pawn and execute shoot function
	
	// if(OwnerComp.GetAIOwner() == nullptr) return; // If there is not AI controller
	// ALightHouse* LightHouse = Cast<ALightHouse>(OwnerComp.GetAIOwner()->GetPawn());
	// if(LightHouse == nullptr) return;

	UE_LOG(LogTemp, Warning, TEXT("Tic Tac"));

	// if(!ensure(SplineComponent)) UE_LOG(LogTemp, Warning, TEXT("%s not found"), *SplineComponent->GetName());
	// if(!ensure(PatrolTargetComponent)) UE_LOG(LogTemp, Warning, TEXT("%s not found"), *PatrolTargetComponent->GetName());
	//
	// float InterpPosition = TimeInterp(DeltaTime);
	// PatrolTargetComponent->SetWorldLocation(SplineComponent->GetLocationAtDistanceAlongSpline(
	// 	InterpPosition*SplineComponent->GetSplineLength(), ESplineCoordinateSpace::World
	// 	));
}

