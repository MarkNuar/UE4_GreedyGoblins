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
	
	NoiseAreaCollider = GetOwner()->FindComponentByClass<USphereComponent>();
	if(!ensure(NoiseAreaCollider != nullptr)) return;
	NoiseAreaCollider->SetSphereRadius(0);
	
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
		{
			Velocity = FVector::ZeroVector;
		}
	}
	else
	{
		Velocity = Velocity + GetOwner()->GetActorForwardVector() * Move.Throttle * Acceleration * Move.DeltaTime;
	}
	
	Velocity = Velocity.GetClampedToMaxSize(MaxSpeed);
	Speed = Velocity.Size();

	UpdateNoiseAreaRadius();
	ApplyRotation(Move.DeltaTime, Move.SteeringThrow);
	UpdateLocationFromVelocity(Move.DeltaTime);
}

void UBoatMovementComponent::ToggleFastMode()
{
	if(!bIsInFastMode)
	{
		bIsInFastMode = true;
		MaxSpeed = FastModeMaxSpeed;
	}
	else
	{
		bIsInFastMode = false;
		MaxSpeed = StealthModeMaxSpeed;
	}
}

FBoatMove UBoatMovementComponent::CreateMove(float DeltaTime) const
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
	{
		OnWallHit(HitResult.Normal, DeltaTime);
	}
}

void UBoatMovementComponent::OnWallHit(FVector HitNormal, float DeltaTime)
{
	const FVector OppositeNormalU = -FVector::VectorPlaneProject(HitNormal, FVector::ZAxisVector).GetSafeNormal(); // Normal vector parallel to 
	const FVector VelocityU = Velocity.GetSafeNormal();
	const float CollisionAngle = FMath::Acos(FVector::DotProduct(OppositeNormalU, VelocityU));
	const float CollisionAngleDegrees = FMath::RadiansToDegrees(CollisionAngle);
		
	if (CollisionAngleDegrees < MinSlideAngle) // 20 degrees limit angle for bounce 
	{
		Velocity = - Bouncyness * OppositeNormalU.ProjectOnTo(GetOwner()->GetActorForwardVector()); // 10 = bouncyness
	}
	else
	{
		const float BoatRotationAngle = (0.8 + PI/2 - CollisionAngle) * DeltaTime;
		const FQuat RotationDelta(FVector::CrossProduct(OppositeNormalU, VelocityU), OnWallSlideRotationSpeed * BoatRotationAngle);
		Velocity = RotationDelta.RotateVector(Velocity);
		GetOwner()->AddActorWorldRotation(RotationDelta);
	}
}


void UBoatMovementComponent::ApplyRotation(float DeltaTime, float LocalSteeringThrow)
{
	float DeltaLocation = FVector::DotProduct(GetOwner()->GetActorForwardVector(), Velocity) * DeltaTime;
	float RotationAngle = DeltaLocation / MinTurningRadius * LocalSteeringThrow;
    FQuat RotationDelta(GetOwner()->GetActorUpVector(), RotationAngle);
    
    Velocity = RotationDelta.RotateVector(Velocity);
	GetOwner()->AddActorWorldRotation(RotationDelta);
}

void UBoatMovementComponent::UpdateNoiseAreaRadius() const
{
	float Radius;
	
	if(bIsInFastMode)
	{
		Radius = Speed * MaxFastModeNoiseAreaRadius / (0.35 * MaxSpeed);
		Radius = FMath::Clamp(Radius, 0.f, MaxFastModeNoiseAreaRadius);
	}
	else
	{
		Radius = Speed * MaxStealthModeNoiseAreaRadius / (0.35 * MaxSpeed);
		Radius = FMath::Clamp(Radius, 0.f, MaxStealthModeNoiseAreaRadius);
	}
	
	NoiseAreaCollider->SetSphereRadius(Radius * 100);
}



