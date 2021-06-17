// Fill out your copyright notice in the Description page of Project Settings.


#include "LighthouseMovementReplicator.h"

#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
ULighthouseMovementReplicator::ULighthouseMovementReplicator()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void ULighthouseMovementReplicator::BeginPlay()
{
	Super::BeginPlay();

	PatrolTargetComponent = GetOwner()->FindComponentByClass<USceneComponent>();
	if(!ensure(PatrolTargetComponent != nullptr)) return;
}

void ULighthouseMovementReplicator::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ULighthouseMovementReplicator, PatrolTargetServerTransform);
}

// Called every frame
void ULighthouseMovementReplicator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// we are the server and in control of the pawn
	if(GetOwner()->GetRemoteRole() == ROLE_SimulatedProxy)
	{
		PatrolTargetServerTransform = PatrolTargetComponent->GetComponentTransform(); 
	}

	// we are somebody else seen by the client (i.e the server or other clients)
	if(GetOwnerRole() == ROLE_SimulatedProxy)
	{
		ClientTick(DeltaTime);
	}
}

void ULighthouseMovementReplicator::OnRep_ServerState() // after server replicated serverstate to everybody (if i'm a client seen from another client's POV)
{
	// if(!ensure(PatrolTargetComponent != nullptr))
	// 	return;
	if(PatrolTargetComponent)
	{
		ClientTimeBetweenLastUpdates = ClientTimeSinceUpdate;
        ClientTimeSinceUpdate = 0;

        ClientStartTransform.SetLocation(PatrolTargetComponent->GetComponentLocation()); //client sets its move after server validated it, taking it from the serverstate struct */
    
        GetOwner()->SetActorTransform(PatrolTargetServerTransform);
	}
}

void ULighthouseMovementReplicator::ClientTick(float DeltaTime)
{
	ClientTimeSinceUpdate += DeltaTime;

	float LerpRatio = ClientTimeSinceUpdate / ClientTimeBetweenLastUpdates;

	FLinearSpline Spline = CreateSpline();
	
	InterpolateLocation(Spline, LerpRatio);	
}

FLinearSpline ULighthouseMovementReplicator::CreateSpline()
{
	FLinearSpline Spline;

	Spline.TargetLocation = PatrolTargetServerTransform.GetLocation();
	Spline.StartLocation = ClientStartTransform.GetLocation();

	return Spline;
}

void ULighthouseMovementReplicator::InterpolateLocation(const FLinearSpline& Spline, float LerpRatio) const
{
	FVector NewLocation = Spline.InterpolateLocation(LerpRatio);
	PatrolTargetComponent->SetWorldLocation(NewLocation);	
}

