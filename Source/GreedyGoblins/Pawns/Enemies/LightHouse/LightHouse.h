// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "LighthouseMovementReplicator.h"
#include "Components/SplineComponent.h"
#include "Components/SpotLightComponent.h"
#include "GameFramework/Pawn.h"
#include "GreedyGoblins/Pawns/Boat/Boat.h"

#include "LightHouse.generated.h"

UCLASS()
class GREEDYGOBLINS_API ALightHouse : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ALightHouse();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FVector GetPositionAlongSpline() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ULighthouseMovementReplicator* MovementReplicator;
	
	USceneComponent* GetPatrolTargetTransform() const
	{
		return PatrolTargetTransform;
	}

	USplineComponent* GetSplineComponent() const
	{
		return SplineComponent;
	}

	float GetLightSpeed() const
	{
		return LightSpeed;
	}

	ABoat* GetBoatToChase() const
	{
		return BoatToChase;
	}
	
	float GetMaxPatrolTargetDistance() const
	{
		return MaxPatrolTargetDistance;
	}

	USceneComponent* GetEyeSocket() const
	{
		return EyeSocket;
	}

	void SetIsChasing(bool val)
	{
		bIsChasing = val;
	};

	bool GetIsChasing() const
	{
		return bIsChasing;
	};

	void SetIsInEnragedMode(bool val)
	{
		bIsInEnragedMode = val;
	}
	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	// Light speed in m/s
	UPROPERTY(EditAnywhere)
	float LightSpeed = 20.0f;

	// Max Distance possible from Patrol Target in METERS
	UPROPERTY(EditAnywhere)
	float MaxPatrolTargetDistance = 200;

	UPROPERTY()
	ABoat* BoatToChase;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* LightConeMeshComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpotLightComponent* SpotLightComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* PatrolTargetTransform;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USplineComponent* SplineComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* EyeSocket;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY()
	UMaterialInstanceDynamic* LightConeDynamicMaterial;

	UPROPERTY(Replicated)
	bool bIsChasing = false;

	UPROPERTY(Replicated)
	bool bIsInEnragedMode = false;

	void UpdateLightColor();
};
