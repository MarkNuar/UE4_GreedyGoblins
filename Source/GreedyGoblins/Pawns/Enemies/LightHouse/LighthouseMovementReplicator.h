// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LighthouseMovementReplicator.generated.h"

USTRUCT()
struct FLinearSpline
{
	GENERATED_USTRUCT_BODY()
	
	FVector StartLocation, TargetLocation;

	FVector InterpolateLocation(float LerpRatio) const
	{
		return FMath::LerpStable(StartLocation,  TargetLocation,  LerpRatio);
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GREEDYGOBLINS_API ULighthouseMovementReplicator : public UActorComponent
{
	GENERATED_BODY()

	
public:	
	// Sets default values for this component's properties
	ULighthouseMovementReplicator();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	
	UPROPERTY()
	USceneComponent* PatrolTargetComponent;

	UPROPERTY(ReplicatedUsing= OnRep_ServerState) //OnRep_Serverstate is a callback function called only AFTER ServerState has been replicated
	FTransform PatrolTargetServerTransform;

	FTransform ClientStartTransform;
	
	float ClientTimeSinceUpdate;
	float ClientTimeBetweenLastUpdates;
	
	UFUNCTION()
	void OnRep_ServerState();

	FLinearSpline CreateSpline();
	void InterpolateLocation(const FLinearSpline& Spline, float LerpRatio) const;
	void ClientTick(float DeltaTime);
};
