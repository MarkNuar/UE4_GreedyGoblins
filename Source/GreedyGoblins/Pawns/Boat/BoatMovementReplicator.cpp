// Fill out your copyright notice in the Description page of Project Settings.


#include "BoatMovementReplicator.h"

#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UBoatMovementReplicator::UBoatMovementReplicator()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicated(true);
}


// Called when the game starts
void UBoatMovementReplicator::BeginPlay()
{
	Super::BeginPlay();
	MovementComponent = GetOwner()->FindComponentByClass<UBoatMovementComponent>();
}

// Called every frame
void UBoatMovementReplicator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
	if (MovementComponent == nullptr) return;
	
	FBoatMove LastMove = MovementComponent->GetLastMove();
	
	// we are the client and in control of the pawn
	if (GetOwnerRole() == ROLE_AutonomousProxy)
	{
		UnacknowledgedMoves.Add(LastMove);
		Server_SendMove(LastMove); // I (client) send my move to the server, THE SERVER EXECUTES IT, and then the server updates its state regarding the client
	}

	// we are the server and in control of the pawn
	if(GetOwner()->GetRemoteRole() == ROLE_SimulatedProxy)
	{
		UpdateServerState(LastMove); // updates the server on the last move of the client
	}

	// we are the server seen by the client
	if(GetOwnerRole() == ROLE_SimulatedProxy)
	{
		ClientTick(DeltaTime);
	}
}

void UBoatMovementReplicator::UpdateServerState(const FBoatMove& Move) //
{
	ServerState.LastMove = Move;
	ServerState.Transform = GetOwner()->GetActorTransform();
	ServerState.Velocity = MovementComponent->GetVelocity();
}

void UBoatMovementReplicator::ClientTick(float DeltaTime) //in order to correctly see the server from the client POV (simulated proxy)
{
	ClientTimeSinceUpdate += DeltaTime;

	if (ClientTimeBetweenLastUpdates < KINDA_SMALL_NUMBER) return;
	if (MovementComponent == nullptr) return;

	float LerpRatio = ClientTimeSinceUpdate / ClientTimeBetweenLastUpdates;

	FHermiteCubicSpline Spline = CreateSpline();
	
	InterpolateLocation(Spline, LerpRatio);
	InterpolateVelocity(Spline, LerpRatio);
	InterpolateRotation(LerpRatio);
}

FHermiteCubicSpline UBoatMovementReplicator::CreateSpline()
{
	FHermiteCubicSpline Spline;

	Spline.TargetLocation = ServerState.Transform.GetLocation();
	Spline.StartLocation = ClientStartTransform.GetLocation();
	Spline.StartDerivative = ClientStartVelocity * VelocityToDerivative();
	Spline.TargetDerivative = ServerState.Velocity * VelocityToDerivative();

	return Spline;
}

void UBoatMovementReplicator::InterpolateLocation(const FHermiteCubicSpline& Spline, float LerpRatio)
{
	FVector NewLocation = Spline.InterpolateLocation(LerpRatio);
	
	if(MeshOffsetRoot != nullptr)
		MeshOffsetRoot->SetWorldLocation(NewLocation);	
}

void UBoatMovementReplicator::InterpolateVelocity(const FHermiteCubicSpline& Spline, float LerpRatio)
{
	FVector NewDerivative = Spline.InterpolateDerivative(LerpRatio);
	FVector NewVelocity = NewDerivative / VelocityToDerivative();
	MovementComponent->SetVelocity(NewVelocity);
}

void UBoatMovementReplicator::InterpolateRotation(float LerpRatio)
{
	FQuat TargetRotation = ServerState.Transform.GetRotation();
	FQuat StartRotation = ClientStartTransform.GetRotation();
	FQuat NewRotation = FQuat::Slerp(StartRotation, TargetRotation, LerpRatio);

	if(MeshOffsetRoot != nullptr)
		MeshOffsetRoot->SetWorldRotation(NewRotation);
}


float UBoatMovementReplicator::VelocityToDerivative()
{
	return ClientTimeBetweenLastUpdates * 100;
}

void UBoatMovementReplicator::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UBoatMovementReplicator, ServerState);
}

void UBoatMovementReplicator::OnRep_ServerState() // called every "NetUpdateFrequency" seconds, after the server replicated the state to everybody
{
	switch (GetOwnerRole())
	{
		case ROLE_AutonomousProxy:
			AutonomousProxy_OnRep_ServerState();
			break;
		case ROLE_SimulatedProxy:
			SimulatedProxy_OnRep_ServerState();
			break;
		default:
			break;
	}
}

void UBoatMovementReplicator::AutonomousProxy_OnRep_ServerState() // after server replicated serverstate to everybody (if i'm a client)
{
	if (MovementComponent == nullptr) return;
	
	GetOwner()->SetActorTransform(ServerState.Transform); //client sets its move after server validated it, taking it from the serverstate struct
	MovementComponent->SetVelocity(ServerState.Velocity);

	ClearAcknowledgeMoves(ServerState.LastMove);

	for (const FBoatMove& Move : UnacknowledgedMoves)
	{
		MovementComponent->SimulateMove(Move);
	}
}

void UBoatMovementReplicator::SimulatedProxy_OnRep_ServerState() // after server replicated serverstate to everybody (if i'm a client seen from another client's POV)
{
	if (MovementComponent == nullptr) return;
	
	ClientTimeBetweenLastUpdates = ClientTimeSinceUpdate;
	ClientTimeSinceUpdate = 0;

	if (MeshOffsetRoot != nullptr)
	{
		ClientStartTransform.SetLocation(MeshOffsetRoot->GetComponentLocation());
		ClientStartTransform.SetRotation(MeshOffsetRoot->GetComponentQuat());
	}
	ClientStartVelocity = MovementComponent->GetVelocity();

	GetOwner()->SetActorTransform(ServerState.Transform); //client sets its move after server validated it, taking it from the serverstate struct
}

void UBoatMovementReplicator::ClearAcknowledgeMoves(FBoatMove LastMove)
{
	TArray<FBoatMove> NewMoves;

	for (const FBoatMove& Move : UnacknowledgedMoves)
	{
		if(Move.Time > LastMove.Time)
			NewMoves.Add(Move);
	}

	UnacknowledgedMoves = NewMoves;
}


void UBoatMovementReplicator::Server_SendMove_Implementation(FBoatMove Move) // I (client) send my move to the server, THE SERVER EXECUTES IT, and then the server updates its state regarding the client
{
	if (MovementComponent == nullptr) return;

	ClientSimulatedTime += Move.DeltaTime;
	MovementComponent->SimulateMove(Move);
	UpdateServerState(Move);
}

bool UBoatMovementReplicator::Server_SendMove_Validate(FBoatMove Move)
{
	float ProposedTime = ClientSimulatedTime + Move.DeltaTime;
	bool ClientNotRunningAhead = ProposedTime < GetWorld()->TimeSeconds;
	
	if(!ClientNotRunningAhead)
	{
		UE_LOG(LogTemp, Error, TEXT("Client is running too fast."));
		return false;
	}
	
	if(!Move.isValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Received invalid move"));
		return false;
	}
	
	return true;
}

