// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SphereComponent.h"

#include "BoatMovementComponent.generated.h"

USTRUCT()
struct FBoatMove
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	float Throttle;
	UPROPERTY()
	float SteeringThrow;
	UPROPERTY()
	float DeltaTime;
	UPROPERTY()
	float Time;

	bool isValid() const
	{
		return FMath::Abs(Throttle) <= 1 && FMath::Abs(SteeringThrow) <= 1;
	}
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GREEDYGOBLINS_API UBoatMovementComponent : public UActorComponent
{
	GENERATED_BODY()

	public:	
	// Sets default values for this component's properties
	UBoatMovementComponent();

	protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void SimulateMove(const FBoatMove& Move);
	void ToggleFastMode();
	
	FVector GetVelocity() const { return Velocity; }
	void SetVelocity(FVector Val) { Velocity = Val; }

	void SetThrottle(float Val) { Throttle = Val; }
	void SetSteeringThrow(float Val) { SteeringThrow = Val; }

	FBoatMove GetLastMove() const { return LastMove; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* NoiseAreaCollider;
	
	private:

	void ApplyRotation(float DeltaTime, float SteeringThrow);
	void UpdateLocationFromVelocity(float DeltaTime);
	FBoatMove CreateMove(float DeltaTime) const;
	void UpdateNoiseAreaRadius() const;
	bool bIsInFastMode = false;
	
	UPROPERTY(EditAnywhere)
	float Acceleration = 5;

	UPROPERTY(EditAnywhere)
	float Deceleration = 5;
	
	// Max Stealth Mode speed in m/s
	UPROPERTY(EditAnywhere)
	float StealthModeMaxSpeed = 25;
	
	// Max Fast Mode speed in m/s
	UPROPERTY(EditAnywhere)
	float FastModeMaxSpeed = 50;

	// Radius of the noise area in stealth mode in meters
	UPROPERTY(EditAnywhere)
	float MaxStealthModeNoiseAreaRadius = 10;

	// Radius of the noise area in fast mode in meters
	UPROPERTY(EditAnywhere)
	float MaxFastModeNoiseAreaRadius = 20;
	
	// Minimum radius of the boat turning circle at full lock (m)
	UPROPERTY(EditAnywhere)
	float MinTurningRadius = 10;

	FVector Velocity;
	
	float MaxSpeed = StealthModeMaxSpeed;
	
	UPROPERTY(VisibleAnywhere)
	float Throttle; // Value between -1 and 1 INTEGER

	UPROPERTY(VisibleAnywhere)
	float SteeringThrow;

	UPROPERTY(VisibleAnywhere)
	float Speed;

	FBoatMove LastMove;

};