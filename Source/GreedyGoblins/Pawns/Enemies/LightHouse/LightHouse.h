// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/CapsuleComponent.h"
#include "Components/SplineComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/TimelineComponent.h"
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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	float TimeInterp(float DeltaTime);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UCapsuleComponent* GetPatrolTargetComponent() const
	{
		return PatrolTargetComponent;
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

private:

	// Light speed in m/s
	UPROPERTY(EditAnywhere)
	float LightSpeed = 5.0f;

	UPROPERTY()
	ABoat* BoatToChase;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* Root;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpotLightComponent* SpotLightComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* PatrolTargetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USplineComponent* SplineComponent;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
