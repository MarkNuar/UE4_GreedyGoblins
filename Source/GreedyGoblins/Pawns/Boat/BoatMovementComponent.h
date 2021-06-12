// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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

	FVector GetVelocity() { return Velocity; }
	void SetVelocity(FVector Val) { Velocity = Val; }

	void SetThrottle(float Val) { Throttle = Val; }
	void SetSteeringThrow(float Val) { SteeringThrow = Val; }

	FBoatMove GetLastMove() { return LastMove; }
	
	private:

	void ApplyRotation(float DeltaTime, float SteeringThrow);
	void UpdateLocationFromVelocity(float DeltaTime);
	FBoatMove CreateMove(float DeltaTime);

	UPROPERTY(EditAnywhere)
	float Acceleration = 5;

	UPROPERTY(EditAnywhere)
	float Deceleration = 5;
	
	//Max  Speed in m/s
	UPROPERTY(EditAnywhere)
	float MaxBaseSpeed = 25;

	// minimum radius of the car turning circle at full lock (m)
	UPROPERTY(EditAnywhere)
	float MinTurningRadius = 10;

	FVector Velocity;

	UPROPERTY(VisibleAnywhere)
	float Throttle; // Value between -1 and 1 INTEGER
	float SteeringThrow;

	UPROPERTY(VisibleAnywhere)
	float Speed;

	FBoatMove LastMove;

};