// Fill out your copyright notice in the Description page of Project Settings.


#include "BoatMovementComponent.h"

// Sets default values for this component's properties
UBoatMovementComponent::UBoatMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBoatMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UBoatMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(GetOwnerRole() == ROLE_AutonomousProxy || GetOwner()->GetRemoteRole() == ROLE_SimulatedProxy)
	{
		LastMove = CreateMove(DeltaTime);
		SimulateMove(LastMove);
	}
}

void UBoatMovementComponent::SimulateMove(const FBoatMove& Move) //The server simulates the moves of the other clients 
{
	FVector Force = GetOwner()->GetActorForwardVector() * MaxDrivingForce * Move.Throttle;

	Force += GetAirResistance();
	Force += GetRollingResistance();

	FVector Acceleration = Force / Mass;
	
	Velocity = Velocity + Acceleration * Move.DeltaTime;
	Speed = Velocity.Size();
	
	ApplyRotation(Move.DeltaTime, Move.SteeringThrow);
	UpdateLocationFromVelocity(Move.DeltaTime);
	
}

FBoatMove UBoatMovementComponent::CreateMove(float DeltaTime)
{
	FBoatMove Move;
	Move.DeltaTime = DeltaTime;
	Move.SteeringThrow = SteeringThrow;
	Move.Throttle = Throttle;
	Move.Time = GetWorld()->TimeSeconds;

	return Move;
}

FVector UBoatMovementComponent::GetAirResistance()
{
	return -Velocity.GetSafeNormal() * Velocity.SizeSquared() * DragCoefficient;
}

FVector UBoatMovementComponent::GetRollingResistance()
{
	float AccelerationDueToGravity = -GetWorld()->GetGravityZ() / 100;
	float NormalForce = Mass * AccelerationDueToGravity;
	return - Velocity.GetSafeNormal() * RollingResistanceCoefficient * NormalForce;
}


void UBoatMovementComponent::UpdateLocationFromVelocity(float DeltaTime)
{
	FVector Translation = Velocity * 100 * DeltaTime;

	FHitResult HitResult;
	GetOwner()->AddActorWorldOffset(Translation, true, &HitResult);

	if (HitResult.IsValidBlockingHit())
		Velocity = FVector::ZeroVector;
}

void UBoatMovementComponent::ApplyRotation(float DeltaTime, float LocalSteeringThrow)
{
	float DeltaLocation = FVector::DotProduct(GetOwner()->GetActorForwardVector(), Velocity) * DeltaTime;
	float RotationAngle = DeltaLocation / MinTurningRadius * LocalSteeringThrow;
    FQuat RotationDelta(GetOwner()->GetActorUpVector(), RotationAngle);
    
    Velocity = RotationDelta.RotateVector(Velocity);
	GetOwner()->AddActorWorldRotation(RotationDelta);
}


