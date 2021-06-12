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

	Velocity = FVector::ZeroVector;
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

	if(abs(Move.Throttle) < SMALL_NUMBER && Velocity.Size() > 0)
	{
		FVector PrevVel = Velocity;
		Velocity = Velocity - FMath::Sign(FVector::DotProduct(GetOwner()->GetActorForwardVector(), Velocity)) * GetOwner()->GetActorForwardVector() * Deceleration * Move.DeltaTime;
		
		if (FVector::DotProduct(PrevVel,Velocity) < 0)
			Velocity = FVector::ZeroVector;
	}
	else
	{
		Velocity = Velocity + GetOwner()->GetActorForwardVector() * Move.Throttle * Acceleration * Move.DeltaTime;
	}
	
	Velocity = Velocity.GetClampedToMaxSize(MaxBaseSpeed);
	
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


