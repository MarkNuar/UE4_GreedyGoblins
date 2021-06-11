// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "BoatMovementComponent.h"
#include "Components/ActorComponent.h"
#include "BoatMovementReplicator.generated.h"


USTRUCT()
struct FBoatState
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FTransform Transform;
	UPROPERTY()
	FVector Velocity;
	UPROPERTY()
	FBoatMove LastMove;

};

USTRUCT()
struct FHermiteCubicSpline
{
	GENERATED_USTRUCT_BODY()
	
	FVector StartLocation, StartDerivative, TargetLocation, TargetDerivative;

	FVector InterpolateLocation(float LerpRatio) const
	{
		return FMath::CubicInterp(StartLocation, StartDerivative, TargetLocation, TargetDerivative, LerpRatio);
	}
	
	FVector InterpolateDerivative(float LerpRatio) const
	{
		return FMath::CubicInterpDerivative(StartLocation, StartDerivative, TargetLocation, TargetDerivative, LerpRatio);
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GREEDYGOBLINS_API UBoatMovementReplicator : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBoatMovementReplicator();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
private:
	void ClearAcknowledgeMoves(FBoatMove LastMove);

	void UpdateServerState(const FBoatMove& Move);

	void ClientTick(float DeltaTime);
	FHermiteCubicSpline CreateSpline();
	
	void InterpolateLocation(const FHermiteCubicSpline& Spline, float LerpRatio);
	void InterpolateVelocity(const FHermiteCubicSpline& Spline, float LerpRatio);
	void InterpolateRotation(float LerpRatio);

	float VelocityToDerivative();
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SendMove(FBoatMove Move); // executed only on server so only the server can validate the move of the client (Client sends move to server)

	UPROPERTY(ReplicatedUsing= OnRep_ServerState)
	FBoatState ServerState;
	
	UFUNCTION()
	void OnRep_ServerState();
	void SimulatedProxy_OnRep_ServerState();
	void AutonomousProxy_OnRep_ServerState();

	TArray<FBoatMove> UnacknowledgedMoves;

	float ClientTimeSinceUpdate;
	float ClientTimeBetweenLastUpdates;
	FTransform ClientStartTransform;
	FVector ClientStartVelocity;

	float ClientSimulatedTime;
	
	UPROPERTY()
	UBoatMovementComponent* MovementComponent;

	UPROPERTY()
	USceneComponent* MeshOffsetRoot;
	UFUNCTION(BlueprintCallable)
	void SetMeshOffsetRoot(USceneComponent* Root) { MeshOffsetRoot = Root; }
	
};
