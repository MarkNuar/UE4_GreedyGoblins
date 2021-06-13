// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "BoatMovementComponent.h"
#include "BoatMovementReplicator.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Pawn.h"
#include "Boat.generated.h"

UCLASS()
class GREEDYGOBLINS_API ABoat : public APawn
{
	GENERATED_BODY()

	public:
	// Sets default values for this pawn's properties
	ABoat();

	protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoatMovementComponent* MovementComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoatMovementReplicator* MovementReplicator;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* BoxCollider;

	private:	
	void MoveForward(float Value);
	void MoveRight(float Value);

	void LookUp(float Value);
	void LookRight(float Value);
	void LookUpRate(float Value);
	void LookRightRate(float Value);
	
	UPROPERTY(EditAnywhere)
	float RotationRate = 10;

	UFUNCTION()
	void OnBoatHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
};
